#include "ground.hpp"
#include "pathmap.hpp"
#include "util.hpp"
#include <pathie/path.hpp>
#include <stdexcept>

using namespace TSC;
using namespace std;
using Pathie::Path;

/**
 * Construct a new level ground object. The `tileset` is a path
 * relative to TSC's data/pixmaps/tilesets directory.
 */
Ground::Ground(const string& tileset)
{
    Path path = Pathmap::GetPixmapsPath() / "tilesets" / tileset;
    if (!path.exists())
        throw(runtime_error(format("Tileset '%s' does not exist", path.utf8_str())));

    sf::Image img;
    img.loadFromFile(path2sf(path));

    // SFML docs say that 512px are minimum of quite old cards,
    // and say that newer cards support 8192px or more.
    unsigned int maxedge = sf::Texture::getMaximumSize();
    if (img.getSize().x > maxedge)
        throw(runtime_error(format("Tileset '%s' is too large for your graphics card, which only supports up to %d pixels for an edge!", path.utf8_str(), maxedge)));
    if (img.getSize().y > maxedge)
        throw(runtime_error(format("Tileset '%s' is too large for your graphics card, which only supports up to %d pixels for an edge!", path.utf8_str(), maxedge)));

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
