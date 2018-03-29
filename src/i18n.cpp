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

#include "i18n.hpp"
#include <errno.h>
#include <string>
#include <SFML/System.hpp>

using namespace std;

// This is the domain translations need to be under for Gettext
// to detect them as belonging to this programme.
#define TSC_GETTEXT_DOMAIN "TSC3"

/// Initialises Gettext. You have to adapt to the environment locale
/// before you call this. `localdir` has to be encoded in UTF-8.
void TSC::SetupI18n(const char* localedir)
{
    if (!bindtextdomain(TSC_GETTEXT_DOMAIN, localedir)) {
        int errsav = errno;
        throw(string("Failed to load translations from path: ") + string(localedir) + ": " + strerror(errsav));
    }

    if (!textdomain(TSC_GETTEXT_DOMAIN)) {
        int errsav = errno;
        throw(string("Failed to bind Gettext domain: ") + strerror(errsav));
    }

    // Force Gettext to return UTF-8, because that's what SFML wants.
    if (!bind_textdomain_codeset(TSC_GETTEXT_DOMAIN, "UTF-8")) {
        int errsav = errno;
        throw(string("Failed to force UTF-8 output from Gettext: ") + strerror(errsav));
    }

}

sf::String S_(const std::string& str)
{
    const char* cstr = gettext(str.c_str());
    std::string cppstr(cstr);
    return sf::String::fromUtf8(cppstr.begin(), cppstr.end());
}
