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

#ifndef TSC_TEXTURE_CACHE_HPP
#define TSC_TEXTURE_CACHE_HPP
#include <SFML/Graphics/Texture.hpp>
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
