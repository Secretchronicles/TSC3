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

#ifndef TSC_CONFIGURATION_HPP
#define TSC_CONFIGURATION_HPP
#include <string>

namespace TSC {

    /**
     * This namespace encapsulates handling of TSC's configuration file.
     */
    namespace Settings {
        void Load();
        void Save();

        extern std::string game_version;
        extern int screen_width;
        extern int screen_height;
        extern int music_volume;
        extern int sound_volume;

        extern bool enable_vsync;
        extern bool enable_always_run;
        extern bool enable_fullscreen;
        extern bool enable_music;
        extern bool enable_sound;
    };

}

#endif /* TSC_CONFIGURATION_HPP */
