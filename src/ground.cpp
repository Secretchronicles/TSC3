/*******************************************************************************
 * This file is part of TSC.
 *
 * TSC is a 2-dimensional platform game.
 * Copyright © 2018 The TSC Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "ground.hpp"
#include "pathmap.hpp"
#include "util.hpp"
#include "xerces_helpers.hpp"
#include <pathie/path.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <stdexcept>
#include <vector>

using namespace TSC;
using namespace std;
using Pathie::Path;
using namespace xercesc;

namespace {
    // Tilset XML settings handler.
    class TilesetSettingsHandler: public xercesc::DefaultHandler
    {
    public:
        TilesetSettingsHandler()
            {
            }
        void startElement(const XMLCh* const,
                          const XMLCh* const xlocalname,
                          const XMLCh* const,
                          const Attributes& attrs)
            {
                string localname(X2U(xlocalname));
                m_chars.clear();

                if (localname == "colrect") {
                    string x = X2U(attrs.getValue(U2X("x")));
                    string y = X2U(attrs.getValue(U2X("y")));
                    string w = X2U(attrs.getValue(U2X("width")));
                    string h = X2U(attrs.getValue(U2X("height")));

                    bboxes.emplace_back(stoi(x), stoi(y), stoi(w), stoi(h));
                }
            }

        void endElement(const XMLCh* const,
                        const XMLCh* const xlocalname,
                        const XMLCh* const)
            {
                string localname(X2U(xlocalname));

                if (localname == "rows") {
                    rows = stoi(m_chars);
                }
                else if (localname == "cols") {
                    cols = stoi(m_chars);
                }
            }

        void characters(const XMLCh* const chars,
                        const XMLSize_t)
            {
                m_chars += X2U(chars);
            }

        int rows;
        int cols;
        vector<sf::FloatRect> bboxes;
    private:
        string m_chars;
    };
}

/**
 * Default constructor that creates an empty Ground object. Call
 * reset() to actually set the ground information.
 */
Ground::Ground()
    : m_rows(0),
      m_cols(0)
{
    //
}

/**
 * Construct a new level ground object.
 *
 * This method may throw an exception when it encounters a problem
 * when inspecting the files related to the tileset.
 *
 * \param tileset
 * Path relative to TSC's data/pixmaps/tilesets directory that gives
 * the tileset you want to use on the ground.
 *
 * \param fields
 * A list of fields (X/Y coordinate positions and tile ids) that give
 * the starting points of each tile and the ID of the tile to draw on
 * it. The width and height of a tile is determined by the size of a
 * tile in the tileset.
 */
Ground::Ground(const string& tileset, const vector<Field>& fields)
    : m_rows(0),
      m_cols(0)
{
    reset(tileset, fields);
}

/**
 * If you used the default constructor, use this function to get the
 * Ground object ready. It takes the same parameters as the parameterised
 * constructor.
 */
void Ground::reset(const string& tileset, const vector<Field>& fields)
{
    Path tileset_path = Pathmap::GetPixmapsPath() / "tilesets" / tileset;
    if (!tileset_path.exists())
        throw(runtime_error(string("Tileset '") + tileset_path.utf8_str() + "' does not exist"));

    Path settings_path = tileset_path.sub_ext(".xml");

    if (!settings_path.exists())
        throw(runtime_error(string("Tileset settings file '") + settings_path.utf8_str() + "' not found"));

    LoadSettingsFile(settings_path.utf8_str());

    sf::Image img;
    img.loadFromFile(path2sf(tileset_path));

    // SFML docs say that 512px are minimum of quite old cards,
    // and say that newer cards support 8192px or more.
    unsigned int maxedge = sf::Texture::getMaximumSize();
    if (img.getSize().x > maxedge)
        throw(runtime_error(format("Tileset '%s' is too large for your graphics card, which only supports up to %d pixels for an edge!", tileset_path.utf8_str(), maxedge)));
    if (img.getSize().y > maxedge)
        throw(runtime_error(format("Tileset '%s' is too large for your graphics card, which only supports up to %d pixels for an edge!", tileset_path.utf8_str(), maxedge)));

    m_tileset.loadFromImage(img);

    ReadVertices(fields, img);
}

void Ground::LoadSettingsFile(const string& path)
{
    unique_ptr<SAX2XMLReader> p_parser(XMLReaderFactory::createXMLReader());
    p_parser->setFeature(XMLUni::fgSAX2CoreValidation, false);

    TilesetSettingsHandler handler;
    p_parser->setContentHandler(&handler);
    p_parser->setErrorHandler(&handler);

    p_parser->parse(U2X(path)); // May throw a Xerces-C parsing exception (which should bubble up)

    m_rows = handler.rows;
    m_cols = handler.cols;
    m_colrects.swap(handler.bboxes);

    if (m_rows <= 0)
        throw(runtime_error("No rows found in the tileset metadata"));
    if (m_rows <= 0)
        throw(runtime_error("No columns found in the tileset metadata"));
}

/**
 * Merges the fields specified when calling the constructor with the information
 * from the tilset, thereby constructing this Ground's vertex array.
 */
void Ground::ReadVertices(const vector<Field>& fields, const sf::Image& img)
{
    // Allocate enough vertices for all the fields
    // (4 vertices for one field required to describe a quad)
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(fields.size() * 4);

    // Calculate the size of one tile (and thereby, one field).
    // The tileset dimensions are required to be an exact multiple.
    int tilewidth  = img.getSize().x / m_cols;
    int tileheight = img.getSize().y / m_rows;

    for(size_t i=0; i < fields.size(); i++) {
        // Define the quad for this field (under the assumption that the entire
        // Ground is at (0|0) -- transformations will take care of moving it around).
        m_vertices[i*4  ].position = sf::Vector2f(fields[i].x,             fields[i].y);
        m_vertices[i*4+1].position = sf::Vector2f(fields[i].x + tilewidth, fields[i].y);
        m_vertices[i*4+2].position = sf::Vector2f(fields[i].x + tilewidth, fields[i].y + tileheight);
        m_vertices[i*4+3].position = sf::Vector2f(fields[i].x,             fields[i].y + tileheight);

        // Map it to a part of the texture of equal dimensions as described by
        // the tile index for this field.
        int row = fields[i].tileid / m_cols;
        int col = fields[i].tileid % m_cols;
        m_vertices[i*4  ].texCoords = sf::Vector2f(col * tilewidth,             row * tileheight);
        m_vertices[i*4+1].texCoords = sf::Vector2f(col * tilewidth + tilewidth, row * tileheight);
        m_vertices[i*4+2].texCoords = sf::Vector2f(col * tilewidth + tilewidth, row * tileheight + tileheight);
        m_vertices[i*4+3].texCoords = sf::Vector2f(col * tilewidth,             row * tilewidth + tileheight);
    }
}

void Ground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;

    target.draw(m_vertices, states);
}
