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

#ifndef TSC_APPLICATION_HPP
#define TSC_APPLICATION_HPP
#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>

namespace TSC {

    // forward-declare
    class Scene;

    // This is the native resolution.
    const int NATIVE_WIDTH = 1920;
    const int NATIVE_HEIGHT = 1080;

    /**
     * The Application class contains the game's most basic setup. It can
     * mostly be seen as an OOP abstraction over the main() function that
     * has a constructor and destructor for proper cleanup. It's a singleton
     * class, which means that only ever one instance of it can be constructed;
     * once done, this instance can be retrieved via the Instance() method. Any
     * further attempt to construct a new instance will cause an exception.
     *
     * Additionally, this class serves as a container for those pieces of
     * global information that were to small to warrant its own global object.
     * For example, this class thus contains the FPS counter. It also holds the
     * scene stack and owns the SFML game window.
     */
    class Application {
    public:
        static Application* Instance();

        Application(int argc, char* argv[]);
        ~Application();

        int MainLoop();
        void Terminate();

        std::unique_ptr<Scene> PopScene();
        void PushScene(std::unique_ptr<Scene> p_scene);

    private:
        bool m_terminate;
        float m_frame_time; // How long executing the last frame took in total, in seconds.
        sf::RenderWindow m_window;
        sf::Clock m_game_clock;
        sf::Text m_fps;
        std::stack<std::unique_ptr<Scene>> m_scene_stack;

        void OpenWindow();
    };
}

#endif /* TSC_APPLICATION_HPP */
