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

#include "font_store.hpp"
#include "application.hpp"
#include "pathmap.hpp"
#include <pathie/path.hpp>
#include <SFML/Graphics.hpp>

using namespace TSC;
using namespace Pathie;

/**
 * Loads all fonts the game needs from disk.
 */
FontStore::FontStore(Pathmap& pathmap)
{
    Path dir = pathmap.GetFontPath();
    NormalFont.loadFromFile(dir.join("DejaVuSans.ttf").utf8_str());
    BoldFont.loadFromFile(dir.join("DejaVuSans-Bold.ttf").utf8_str());
    MonospaceFont.loadFromFile(dir.join("DejaVuSansMono.ttf").utf8_str());
    MonospaceBoldFont.loadFromFile(dir.join("DejaVuSansMono-Bold.ttf").utf8_str());
}
