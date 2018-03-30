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

#include "timer.hpp"

using namespace TSC;

/// Default constructor constructs a stopped timer. You need
/// to call Reset() to actually use it.
Timer::Timer()
    : m_duration_ms(0),
      m_active(false)
{
    //
}

/**
 * Construct a timer and start it in one go. This constructor
 * does the same as if you called the default constructor
 * immediately followed by a call to Reset().
 */
Timer::Timer(int duration)
    : m_duration_ms(duration),
      m_active(true)
{
    //
}

/**
 * Is the timer ticking? Call Stop() to stop a ticking timer.
 */
bool Timer::IsActive()
{
    return m_active;
}

/**
 * Has the specified time passed?
 *
 * \remark For stopped timers (default constructor, or Stop()
 * has been called), this method always returns true.
 */
bool Timer::IsOver()
{
    if (!m_active)
        return true;

    return m_clock.getElapsedTime().asMilliseconds() > m_duration_ms;
}

/**
 * Halt the timer. IsActive() will return false afterwards.
 *
 * \remark A timer cannot currently be paused. If you call Stop(), all
 * information about the current state of the timer is discarded.
 */
void Timer::Stop()
{
    m_duration_ms = 0;
    m_active = false;
}

/**
 * Reconfigure the timer to tick for the specified amount of
 * milliseconds.
 */
void Timer::Reset(int duration)
{
    m_duration_ms = duration;
    m_active = true;
    m_clock.restart();
}

/**
 * Returns how many percent of the configured time duration
 * have already passed since the timer was started. Very
 * useful for gradually fading things in or out.
 *
 * \returns A value between 0 (just started) and 100 (finished).
 * Note that this method never returns a value greater than 100;
 * if more time has passed than was configured, this method will
 * return 100 nevertheless.
 */
float Timer::PercentOver()
{
    float p = static_cast<float>(m_clock.getElapsedTime().asMilliseconds()) / static_cast<float>(m_duration_ms);
    if (p > 100.0f)
        return 100.0f;
    else
        return p * 100.0f;
}
