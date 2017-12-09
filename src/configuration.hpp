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

#ifndef TSC_CONFIGURATION_HPP
#define TSC_CONFIGURATION_HPP
#include <string>

namespace TSC {

    class Configuration {
    public:
        Configuration(std::string path);
        ~Configuration();

        inline std::string GetPath() { return m_path; }

        // Actual configuration settings
        std::string game_version;
        int screen_width{640};
        int screen_height{480};
        int screen_bpp{32};
        int music_volume{100};
        int sound_volume{100};

        bool enable_vsync{false};
        bool enable_always_run{false};
        bool enable_fullscreen{false};
        bool enable_music{true};
        bool enable_sound{true};
    private:
        std::string m_path;
    };

}

#endif /* TSC_CONFIGURATION_HPP */
