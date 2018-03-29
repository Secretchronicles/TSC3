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

#ifndef TSC_I18N_HPP
#define TSC_I18N_HPP

#include "gettext.h"
#include <SFML/System/String.hpp>

// Pass this string to Gettext for translation.
// Returns const char* encoded in UTF-8.
#define _(String) gettext(String)
// Pass this string to Gettext for translation.
// Returns an sf::String (UTF-32 encoded) suitable for direct
// use anywhere SFML expects a string to draw.
// Note: This needs to be a function due to the sf::String conversion.
sf::String S_(const std::string& str);
// Same as _(), but for strings that need pluralisation.
#define PL_(Singular, Plural, Num) ngettext((Singular), (Plural), (Num))
// Translates with context where ambigous (see section 11.2.5 of
// the Gettext manual).
#define C_(Context, String) pgettext(Context, String)

namespace TSC {

    void SetupI18n(const char* localedir);

}

#endif
