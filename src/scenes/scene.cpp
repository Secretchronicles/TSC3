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

/*
 * This function contains the event loop, i.e. it retrieves all events
 * SFML accumulated for the given window and then calls the virtual
 * ProcessEvent() function to let the scene handle a specific event.
 *
 * This function also handles global events that always do the same
 * regardless of the current scene.
 */
void Scene::ProcessEvents(sf::RenderWindow* p_window)
{
    sf::Event event;
    while (p_window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed: // Window closed
            gp_app->Terminate();
        default:
            ProcessEvent(event);
        }
    }
}
