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

#ifndef TSC_SCENE_HPP
#define TSC_SCENE_HPP

// forward-declare
namespace sf {
    class RenderWindow;
    class Event;
}

namespace TSC {

    class Scene {
    public:
        Scene() {}
        virtual ~Scene() {}

        /// Override this function if you need user input.
        /// It gets passed one event at a time.
        virtual void ProcessEvent(sf::Event&);

        /**
         * Update() shall update all logic in the scene, but not
         * draw anything onto the screen. Returning false from this
         * method causes the main loop to pop the scene from the
         * scene stack without a subsequent call to Draw().
         */
        virtual bool Update() = 0;
        /// Draw() shall draw the updated scene onto the screen.
        /// Game logic updates should be in Update().
        virtual void Draw(sf::RenderWindow* p_stage) const = 0;
    };

}

#endif /* TSC_SCENE_HPP */
