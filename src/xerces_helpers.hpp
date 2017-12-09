/*******************************************************************************
 * This file is part of TSC.
 *
 * TSC is a 2-dimensional platform game.
 * Copyright © 2017 The TSC Contributors
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

#ifndef TSC_XERCES_HELPERS_HPP
#define TSC_XERCES_HELPERS_HPP
#include <string>
#include <memory>
#include <xercesc/util/XMLString.hpp>

namespace TSC {

    std::string xstr_to_utf8(const XMLCh* xstr);
    std::unique_ptr<XMLCh[]> utf8_to_xstr(const std::string& utf8);

}

#endif /* TSC_XERCES_HELPERS_HPP */
