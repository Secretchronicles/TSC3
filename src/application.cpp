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
#include "audio.hpp"
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

    PushScene(unique_ptr<TitleScene>(new TitleScene()));

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

        p_scene->DoGUI(m_window);
        p_scene->Update(m_window);

        // Update audio system (especially for fading)
        Audio::Update();

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

/**
 * Removes the topmost scene from the stack and returns it.
 * Usually, you don't want to use this method; see the Scene
 * class' docs. Instead, call Scene::Finish() on your scene so the
 * mainloop takes care of popping your current scene off the
 * stack.
 *
 * \returns The scene that was on the top of the scene stack.
 *
 * \remark If you do use this method, be very careful about the
 * return value. If you call it from the currently active scene,
 * it will return the pointer to the very scene that is currently
 * executing, i.e., it's return value will be equal to the
 * `this` pointer, just wrapped in a std::unique_ptr. If you
 * let it go out of scope in such a situation (e.g., by not
 * catering for the return value), unique_ptr's implementation will
 * free the memory occupied by it, i.e., it will delete your
 * `this` pointer while you're still executing a method on it!
 * That is going to cause a segfault for obvious reasons, so, if
 * calling this method from the active scene, do store the return
 * value until you are done with your scene, then let it run out
 * of scope to have unique_ptr clean things up for you.
 *
 * \see PushScene()
 */
unique_ptr<Scene> Application::PopScene()
{
    /* Vampires! Because std::stack doesn't support
     * an atomic remove-from-top-and-return-that
     * operation, this code draws out the the raw
     * pointer from the unique_ptr at the top of
     * the scene stack, then pops the now-empty unique_ptr
     * off the stack, and then returns a newly constructed
     * unique_ptr with the former top element. */
    unique_ptr<Scene>& currscene = m_scene_stack.top();
    Scene* rawptr = currscene.release(); // leaves currscene with a nullptr
    m_scene_stack.pop();
    return unique_ptr<Scene>(rawptr);
}

/**
 * Pushes a new scene onto the scene stack. This method takes
 * ownership of the object, i.e. you can't use what you passed
 * as argument anymore after it returns (it uses std::move()).
 *
 * \remark C++ is smart enough to know that you can't reuse your
 * unique_ptr if you construct it as the argument to this method.
 * That is, you can ommit the explicit call to std::move() on
 * the argument if you use this method like this:
 *
 * ~~~~~~~~~~~{c++}
 * PushScene(unique_ptr<YourNextScene>(new YourNextScene()))
 * ~~~~~~~~~~~
 *
 * If on the other hand you have pre-constructed your scene,
 * you need to signal C++ that it's okay to give up the
 * contents of the unique_ptr by wrapping the argument in
 * std::move().
 *
 * ~~~~~~~~~~~{c++}
 * unique_ptr<YourNextScene> pre_existing(new YourNextScene())
 * PushScene(move(pre_existing))
 * // pre_existing now contains a nullptr!
 * ~~~~~~~~~~~
 *
 * \see PopScene().
 */
void Application::PushScene(unique_ptr<Scene> p_scene)
{
    m_scene_stack.push(move(p_scene));
}
