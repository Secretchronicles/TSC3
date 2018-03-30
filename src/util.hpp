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

#ifndef TSC_UTIL_HPP
#define TSC_UTIL_HPP
#include <string>
#include <SFML/System/String.hpp>

// forward-declare
namespace Pathie {
    class Path;
}

namespace TSC {
    void warn(const std::string& msg);
    std::string format(const std::string& spec, ...);
    sf::String sformat(const std::string& spec, ...);
    sf::String utf82sf(const std::string& utf8);
    sf::String path2sf(const Pathie::Path& path);
}

#endif /* TSC_UTIL_HPP */
