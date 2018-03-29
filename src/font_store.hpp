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

#include <SFML/Graphics.hpp>

namespace TSC {

    // forward-declare
    class Pathmap;

    /**
     * A simple class that does nothing other than loading all of the game's
     * fonts from disk and keeping them around for the entire lifetime of the
     * game. Access to font members directly.
     */
    class FontStore
    {
    public:
        FontStore(Pathmap& pathmap);

        sf::Font NormalFont;        ///< Normal game font.
        sf::Font BoldFont;          ///< Normal game font, bold variant.
        sf::Font MonospaceFont;     ///< Monospace font.
        sf::Font MonospaceBoldFont; ///< Monospace font, bold variant.
    };

}
