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

#include "scene.hpp"
#include "../application.hpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace TSC;

/**
 * Override this function if you need user input. It gets
 * passed one event at a time.
 *
 * Because this function in its implementation in the Scene class also
 * handles global events that always do the same regardless of the
 * current scene, you are required to call the parent class’ implementation
 * of this method when overriding this.
 */
void Scene::ProcessEvent(sf::Event& event)
{
    switch (event.type) {
    case sf::Event::Closed: // Window closed
        Application::Instance()->Terminate();
        break;
    default:
        // Nothing
        break;
    }
}
