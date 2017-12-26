#ifndef TSC_TEXTURE_CACHE_HPP
#define TSC_TEXTURE_CACHE_HPP
#include <SFML/Texture.hpp>
#include <map>
#include <string>

namespace TSC {

    class Pathmap;

    class TextureCache {
    public:
        TextureCache();
        ~TextureCache();

        sf::Texture& Get(const std::string& relpath);
    private:
        std::map<std::string, sf::Texture> m_cache;
    };
}

#endif /* TSC_TEXTURE_CACHE_HPP */
