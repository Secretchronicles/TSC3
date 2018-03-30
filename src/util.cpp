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

#include "util.hpp"
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <SFML/System.hpp>

using namespace std;

void TSC::warn(const std::string& msg)
{
    cerr << "Warning: " << msg << endl;
}

static string vaformat(const string& spec, va_list& ap)
{
    int len      = spec.size() + 4096; // 4096 are an educated guess for what's likely needed
    char* target = (char*) malloc(len);

    // Can't iterate a va_list twice, hence copy it.
    va_list copyap;
    va_copy(copyap, ap);

    int result = vsnprintf(target, len, spec.c_str(), copyap);


    if (result >= len) { // Guess was to small, reallocate with required space
        len    = result + 1; // +1 for terminating NUL
        target = (char*) realloc(target, len);

        result = vsnprintf(target, len, spec.c_str(), ap);

        if (result >= len) { // Should not happen
            free(target);
            throw(runtime_error("format() failed for unknown reasons in vsnprintf()"));
        }
        else if (result < 0) {
            free(target);
            throw(runtime_error("format() failed with an output eror"));
        }
    }
    else if (result < 0) {
        free(target);
        throw(runtime_error("format() failed with an output eror"));
    }

    string retval(target, result);
    free(target);
    return retval;
}

/**
 * A function equivalent to C's sprintf(), but returns a C++ std::string
 * instead so that you don't have to think about the memory management.
 * This function calls vsnprintf() for the actual formatting operation, so
 * look in that function's documentation for the format specifiers.
 */
string TSC::format(const string& spec, ...)
{
    va_list ap;
    va_start(ap, spec);
    std::string result = vaformat(spec, ap);
    va_end(ap);

    return result;
}

/// The same as format(), but returns the result as a UTF-32-encoded
/// sf::String instead that can be directly passed to SFML's functions.
sf::String TSC::sformat(const string& spec, ...)
{
    va_list ap;
    va_start(ap, spec);
    std::string str = vaformat(spec, ap);
    va_end(ap);

    return utf82sf(str);
}

/**
 * Online convenience function for converting a UTF-8 encoded
 * std::string into SFML's sf::String format (which uses UTF-32).
 */
sf::String TSC::utf82sf(const string& utf8)
{
    return sf::String::fromUtf8(utf8.begin(), utf8.end());
}
