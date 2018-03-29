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

/* This file configures the nuklear GUI toolkit using its preprocessor
 * macros as outlined in the nuklear docs. Always include this header
 * file instead of including nuklear directly, so that it is ensured that
 * you always load it with the exact same configuration. The "implementation mode"
 * include as described in the nuklear docs happens in gui.cpp.
 */

#ifndef TSC_GUI_H
#define TSC_GUI_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_ZERO_COMMAND_MEMORY
#define NK_MEMSET memset
#define NK_MEMCPY memcpy
#define NK_SQRT sqrt
#define NK_SIN sinf
#define NK_COS cosf
#define NK_VSNPRINTF vsnprintf

#include <cstring>
#include <cmath>

// nuklear.h contains a number of unused functions with the above configuration.
// Seeing warnings for these is only annoying, so disable them for just nuklear.h.
// The #pragma is GCC-specific, so needs to be wrapped in #ifdef __GNUC__to not
// confuse other compilers.
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <nuklear.h>
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wunused-function"
#endif

// forward-declare
namespace sf {
    class RenderWindow;
}

namespace TSC {
    /// Normal GUI font size in pixels.
    const int GUI_FONT_SIZE = 16;

    void DrawNKGUI(struct nk_context* p_ctx, sf::RenderWindow* p_window);
    float CalculateGUIFontWidth(nk_handle handle, float height, const char* text, int textlen);
}

#endif
