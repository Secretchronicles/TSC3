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

#include "application.hpp"
#include "pathmap.hpp"
#include "configuration.hpp"
#include "scenes/title_scene.hpp"
#include <SFML/Graphics.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace TSC;
using namespace std;

// extern declarations
Application* TSC::gp_app = nullptr;

Application::Application(int argc, char* argv[])
    : mp_window(nullptr),
      mp_config(nullptr),
      mp_pathmap(nullptr),
      m_terminate(false)
{
    xercesc::XMLPlatformUtils::Initialize();

    mp_pathmap = new Pathmap();
    mp_config = new Configuration(mp_pathmap->GetConfigPath());
}

Application::~Application()
{
    if (mp_window)
        delete mp_window;
    if (mp_config)
        delete mp_config;
    if (mp_pathmap)
        delete mp_pathmap;

    xercesc::XMLPlatformUtils::Terminate();
}

int Application::MainLoop()
{
    OpenWindow();
    m_scene_stack.push(unique_ptr<TitleScene>(new TitleScene()));

    // Game main loop
    while (!m_terminate && !m_scene_stack.empty()) {
        unique_ptr<Scene>& p_scene = m_scene_stack.top();

        // Poll events from SFML
        p_scene->ProcessEvents(mp_window);

        // Allow a scene to destroy itself by returning false from Update().
        if (p_scene->Update()) {
            mp_window->clear(sf::Color::Black);
            p_scene->Draw(mp_window);
            mp_window->display();
        }
        else
            m_scene_stack.pop();
    }

    // If the mainloop was ended by m_terminate, end all the scenes
    // that still exist.
    if (!m_scene_stack.empty()) {
        m_scene_stack.pop();
    }

    mp_window->close();

    return 0;
}

// Advises the programme to terminate the next time the main loop runs.
void Application::Terminate()
{
    // Implementing this function by emptying the scene stack right
    // here is probably not a good idea. If a scene calls it, that
    // would lead to the scene being deleted while a function of the
    // scene is still running.
    m_terminate = true;
}

void Application::OpenWindow()
{
    // Retrieve desired resolution from config. If that resolution is not
    // native to the graphics card, override the config with the best native
    // resolution available.
    sf::VideoMode mode(mp_config->screen_width, mp_config->screen_height);
    if (!mode.isValid()) {
            mode = sf::VideoMode::getFullscreenModes()[0];
            mp_config->screen_width = mode.width;
            mp_config->screen_height = mode.height;
    }

    // Create the window
    mp_window = new sf::RenderWindow(mode, "The Secret Chronicles of Dr. M.");

    // Enable vsync if requested
    if (mp_config->enable_vsync) {
        mp_window->setVerticalSyncEnabled(true);
    }
}
