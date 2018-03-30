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

#include "audio.hpp"
#include "pathmap.hpp"
#include "timer.hpp"
#include "util.hpp"
#include <SFML/Audio.hpp>
#include <memory>

using namespace TSC;
using namespace std;

unique_ptr<sf::Music> sp_background_music(new sf::Music);
unique_ptr<sf::Music> sp_next_music(new sf::Music);
Timer s_fade_timer;

/**
 * Play the given music as background music.
 *
 * \param relpath
 * Path to the music, relative to TSC's music/ directory.
 *
 * \param fade (default: false)
 * Whether or not to fade this music in. If set, the current music
 * is slowly faded out while the new music is faded in.
 *
 * \remark Fading takes about 5 seconds to complete. Don't call this method
 * while fading is in progress, it will produce unexpected results.
 */
void Audio::PlayMusic(const std::string& relpath, bool fade)
{
    if (!s_fade_timer.IsActive()) {
        string music_path(Pathmap::GetMusicPath().join(relpath).utf8_str());

        if (fade) {
            sp_next_music->stop();
            sp_next_music->openFromFile(utf82sf(music_path));
            sp_next_music->setLoop(true);
            sp_next_music->setVolume(0);
            sp_next_music->play();

            s_fade_timer.Reset(5000);
        } else {
            sp_background_music->stop();
            sp_background_music->openFromFile(utf82sf(music_path));
            sp_background_music->setLoop(true);
            sp_background_music->setVolume(100);
            sp_background_music->play();
        }
    } else {
        /* Ooops. Currently a music is fading in already.
         * There's nothing what one could sensibly do here.
         * Thus, just reset everything to as it was before fading,
         * then exchange the fading target and redo. */
        sp_background_music->setVolume(100);
        sp_next_music->setVolume(0);
        sp_next_music->stop();
        s_fade_timer.Stop();
        PlayMusic(relpath, true);
    }
}

void Audio::PlaySound(const std::string&)
{
    // TODO
}

/**
 * Update the audio system. Call this once a frame. This is required
 * because the fading uses an internal timer that needs to be checked
 * for adjusting the volume during background music fading.
 */
void Audio::Update()
{
    if (s_fade_timer.IsActive()) {

        sp_background_music->setVolume(100.0f - s_fade_timer.PercentOver());
        sp_next_music->setVolume(s_fade_timer.PercentOver());

        if (s_fade_timer.IsOver()) {
            s_fade_timer.Stop();
            sp_background_music.swap(sp_next_music);
            sp_next_music->stop();
        }
    }
}
