#include "texture_cache.hpp"
#include "application.hpp"
#include "pathmap.hpp"
#include <SFML/Graphics.hpp>

using namespace TSC;
using namespace Pathie;

TextureCache::TextureCache()
{
    //
}

TextureCache::~TextureCache()
{
    //
}

sf::Texture& TextureCache::Get(const std::string& relpath)
{
    if (m_cache.count(relpath) > 0)
        return m_cache[relpath];
    else {
        Path p = gp_app->GetPathmap().GetPixmapsPath() / relpath;
        m_cache[relpath].loadFromFile(p.utf8_str());
        return m_cache[relpath];
    }
}
