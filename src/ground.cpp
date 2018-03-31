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
#include <tuple>

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
        vector<tuple<int, int, int, int>> bboxes;
    private:
        string m_chars;
    };
}

/**
 * Construct a new level ground object. The `tileset` is a path
 * relative to TSC's data/pixmaps/tilesets directory.
 */
Ground::Ground(const string& tileset)
    : m_rows(0),
      m_cols(0)
{
    Path tileset_path = Pathmap::GetPixmapsPath() / "tilesets" / tileset;
    if (!tileset_path.exists())
        throw(runtime_error(format("Tileset '%s' does not exist", tileset_path.utf8_str())));

    Path settings_path = tileset_path.sub_ext(".xml");
    if (!settings_path.exists())
        throw(runtime_error(format("Tileset settings file '%s' not found", settings_path.utf8_str())));

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

    // DEBUG: Testing
    m_vertices.resize(4);
    m_vertices.setPrimitiveType(sf::Quads);

    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(256, 0);
    m_vertices[2].position = sf::Vector2f(256, 256);
    m_vertices[3].position = sf::Vector2f(0, 256);

    m_vertices[0].texCoords = sf::Vector2f(0, 0);
    m_vertices[1].texCoords = sf::Vector2f(256, 0);
    m_vertices[2].texCoords = sf::Vector2f(256, 256);
    m_vertices[3].texCoords = sf::Vector2f(0, 256);
}

void Ground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;

    target.draw(m_vertices, states);
}
