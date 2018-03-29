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

// forward-declare
namespace sf {
    class RenderWindow;
    class Text;
    class Clock;
}

namespace TSC {

    // forward-declare
    class Scene;

    class Application {
    public:
        Application(int argc, char* argv[]);
        ~Application();

        inline const sf::RenderWindow& GetWindow()
        {
            return *mp_window;
        }

        int MainLoop();
        void Terminate();

        std::unique_ptr<Scene> PopScene();
        void PushScene(std::unique_ptr<Scene> p_scene);

    private:
        sf::RenderWindow* mp_window;
        sf::Clock* mp_game_clock;
        sf::Text* mp_fps;
        bool m_terminate;
        float m_frame_time; // How long executing the last frame took in total, in seconds.
        std::stack<std::unique_ptr<Scene>> m_scene_stack;

        void OpenWindow();
    };

    /**
     * This is the one and only global pointer in TSC.
     * It refers to the only Application instance around
     * during programme run. This pointer is set to null
     * while startup is not finished and only assigned
     * after the Application instance has successfully
     * been constructed.
     */
    extern Application* gp_app;

}

#endif /* TSC_APPLICATION_HPP */
