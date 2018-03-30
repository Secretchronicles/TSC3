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

#ifndef TSC_TIMER_HPP
#define TSC_TIMER_HPP
#include <SFML/System/Clock.hpp>

namespace TSC {

    /**
     * A lightweight timer class. Once configured for a specific
     * duration (in milliseconds), it is said to be "active", or
     * "ticking" if you want. It records the moment it was started,
     * and once from the duration of time configured has elapsed,
     * IsOver() will return true. During that period, IsActive()
     * returns true as well, and it will continue to return true
     * even after the amount of time configured has passed.
     *
     * Instances of this class do not have to do anything with threads.
     * All they do is to record a start timestamp, and each time you
     * query the instance, the current time is compared against that
     * timestamp. As a consequence, there's no possibility to specify
     * a callback somewhere. The intentional use of this class is in
     * the Update() routine of your scene, where you act accordingly
     * once you see the timer has finished.
     */
    class Timer
    {
    public:
        Timer();
        Timer(int duration);

        bool IsActive();
        bool IsOver();
        void Stop();
        void Reset(int duration);
        float PercentOver();
    private:
        int m_duration_ms;
        bool m_active;
        sf::Clock m_clock;
    };

}

#endif /* TSC_TIMER_HPP */
