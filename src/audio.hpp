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

#ifndef TSC_AUDIO_HPP
#define TSC_AUDIO_HPP
#include <string>

namespace TSC {

    /**
     * This is TSC's audio system. It's currently pretty simple.
     * Note it does not currently play sounds; see
     * <https://www.sfml-dev.org/tutorials/2.4/audio-sounds.php>
     * how to play sounds directly with SFML. For music, please do
     * not use that facilities, but use the Audio::PlayMusic() function
     * instead, which will do proper music fading when required.
     */
    namespace Audio {
        void PlayMusic(const std::string& relpath, bool fade = false);
        void Update();
    }

}

#endif /* TSC_AUDIO_HPP */
