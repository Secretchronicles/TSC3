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
    class Font;
    class Event; // The SFML docs incorrectly state that this is a union; it isn't, it's a class containing a member and an unnamed union.
    class RenderWindow;
}

namespace TSC {

    /**
     * This namespace contains the GUI system, i.e. the in-game system for the menus and such.
     * It doesn't have anything to do with setting up the game window itself, that falls into
     * SFML's responsibility and is done in application.cpp.
     *
     * The order in which functions from this namespace have to be called is:
     *
     * 1. Init()
     * 2. In the main loop:
     *   1. ProcessEvent() for every event received from the user
     *   2. Get(), everytime you want to construct a GUI window, typically in a scene
     *   3. Draw()
     * 3. Cleanup()
     *
     * The Application class is responsible for calling everything but the Get() function,
     * which should be called from the respective scenes.
     */
    namespace GUI {
        /// Normal GUI font size in pixels.
        const int NORMAL_FONT_SIZE = 16;

        void Init();
        void Cleanup();
        void ProcessEvent(sf::Event& event);
        void Draw(sf::RenderWindow& window);

        nk_context* Get();

        void Enable();
        void Disable();
        void Toggle();
        bool IsEnabled();

        // The fonts used in the game.
        extern sf::Font NormalFont;
        extern sf::Font BoldFont;
        extern sf::Font MonospaceFont;
        extern sf::Font MonospaceBoldFont;
    }
}

#endif
