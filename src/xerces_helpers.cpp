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

#include "xerces_helpers.hpp"
#include <xercesc/util/TransService.hpp>
#include <cstring>

using namespace xercesc;
using namespace std;

/**
 * Converts the given Xerces-C string into a UTF-8 string.
 *
 * \see X2U() macro.
 */
string TSC::xstr_to_utf8(const XMLCh* xstr)
{
    TranscodeToStr transcoder(xstr, "UTF-8");
    return string(reinterpret_cast<const char*>(transcoder.str()));
}

/**
 * Converts the given UTF-8 string into a Xerces-C string.
 * The return value of this method is guaranteed to have
 * a terminal NUL.
 *
 * \see U2X() macro.
 */
unique_ptr<XMLCh[]> TSC::utf8_to_xstr(const std::string& utf8)
{
    TranscodeFromStr transcoder(reinterpret_cast<const XMLByte*>(utf8.c_str()), utf8.length(), "UTF-8");
    unique_ptr<XMLCh[]> buf(new XMLCh[transcoder.length()+1]);

    memset(buf.get(), '\0', sizeof(XMLCh) * (transcoder.length() + 1));
    memcpy(buf.get(), transcoder.str(), sizeof(XMLCh) * transcoder.length());

    return buf;
}
