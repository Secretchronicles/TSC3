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

#include "application.hpp"
#include "pathmap.hpp"
#include "configuration.hpp"
#include "scenes/title_scene.hpp"
#include <SFML/Window.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace TSC;
using namespace std;

// extern declarations
Application* TSC::gp_app = nullptr;

Application::Application(int argc, char* argv[])
    : mp_window(nullptr),
      mp_config(nullptr),
      mp_pathmap(nullptr)
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
    m_scene_stack.push(unique_ptr<TitleScene>(new TitleScene()));

    // Game main loop
    while (!m_scene_stack.empty()) {
        unique_ptr<Scene>& p_scene = m_scene_stack.top();

        // Allow a scene to destroy itself by returning false from update().
        if (p_scene->Update())
            p_scene->Draw();
        else
            m_scene_stack.pop();
    }

    return 0;
}
