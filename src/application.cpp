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
#include "settings.hpp"
#include "scenes/title_scene.hpp"
#include "texture_cache.hpp"
#include "gui.hpp"
#include "util.hpp"
#include "i18n.hpp"
#include <SFML/Graphics.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace TSC;
using namespace std;

static Application* sp_app = nullptr;

/**
 * Returns the singleton instance of this class. Note that this method
 * returns a nullptr until the constructor has returned.
 */
Application* Application::Instance()
{
    return sp_app;
}

Application::Application(int argc, char* argv[])
    : m_terminate(false),
      m_frame_time(0.0f)
{
    if (sp_app)
        throw(std::runtime_error("Can't have more than one Application instance!"));

    SetupI18n(); // Always call this first. It sets the programme's global locale.
    xercesc::XMLPlatformUtils::Initialize();

    Settings::Load();
    GUI::Init();

    sp_app = this;
}

Application::~Application()
{
    GUI::Cleanup();
    Settings::Save();

    xercesc::XMLPlatformUtils::Terminate();
}

int Application::MainLoop()
{
    OpenWindow();

    m_fps.setFont(GUI::NormalFont);
    m_fps.setFillColor(sf::Color::Yellow);
    m_fps.setCharacterSize(TSC::GUI::NORMAL_FONT_SIZE);
    m_fps.setPosition(10, 10);

    m_scene_stack.push(unique_ptr<TitleScene>(new TitleScene()));

    // Game main loop
    while (!m_terminate && !m_scene_stack.empty()) {
        m_frame_time = m_game_clock.restart().asSeconds();
        unique_ptr<Scene>& p_scene = m_scene_stack.top();

        // Remove top scene and redo if it's finished.
        if (p_scene->HasFinished()) {
            m_scene_stack.pop();
            continue;
        }

        // Poll events from SFML
        sf::Event event;
        while (m_window.pollEvent(event)) {
            GUI::ProcessEvent(event);
            p_scene->ProcessEvent(event);
        }

        p_scene->DoGUI();
        p_scene->Update(m_window);

            // Draw scene
        m_window.clear(sf::Color::Black);
        p_scene->Draw(m_window);

        // Draw GUI on top of it
        GUI::Draw(m_window);

        // Draw FPS
        int fps = static_cast<int>(1.0f / m_frame_time);
        m_fps.setString(sformat(_("FPS: %d"), fps));
        m_window.draw(m_fps);

        // Flip buffers
        m_window.display();

        // Late update for special tasks.
        p_scene->LateUpdate();
    }

    // If the mainloop was ended by m_terminate, end all the scenes
    // that still exist.
    if (!m_scene_stack.empty()) {
        m_scene_stack.pop();
    }

    m_window.close();

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
    sf::VideoMode mode(Settings::screen_width, Settings::screen_height);
    if (!mode.isValid()) {
            mode = sf::VideoMode::getFullscreenModes()[0];
            Settings::screen_width = mode.width;
            Settings::screen_height = mode.height;
    }

    // TRANS: This is the window's title.
    m_window.create(mode, _("The Secret Chronicles of Dr. M."));

    // Enable vsync if requested
    if (Settings::enable_vsync) {
        m_window.setVerticalSyncEnabled(true);
    }
}
