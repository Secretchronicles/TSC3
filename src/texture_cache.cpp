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

#include "texture_cache.hpp"
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

/**
 * Retrieve a texture from the list of loaded textures. If the texture
 * has not yet been loaded, does so and places it in the list of loaded
 * textures.
 *
 * \param relpath
 * Path to the file to load as a texture. This needs to be relative
 * to the pixmaps/ directory.
 */
sf::Texture& TextureCache::Get(const std::string& relpath)
{
    if (m_cache.count(relpath) > 0)
        return m_cache[relpath];
    else {
        Path p = Pathmap::GetPixmapsPath() / relpath;
        m_cache[relpath].loadFromFile(p.utf8_str());
        return m_cache[relpath];
    }
}
