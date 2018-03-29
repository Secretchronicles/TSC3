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
struct nk_context;
struct nk_user_font;
namespace sf {
    class RenderWindow;
}

namespace TSC {

    // forward-declare
    class Scene;
    class Configuration;
    class Pathmap;
    class TextureCache;
    class FontStore;

    class Application {
    public:
        Application(int argc, char* argv[]);
        ~Application();

        inline const sf::RenderWindow& GetWindow()
        {
            return *mp_window;
        }

        inline const Pathmap& GetPathmap()
        {
            return *mp_pathmap;
        }

        inline const Configuration& GetConfig()
        {
            return *mp_config;
        }

        inline TextureCache& GetTextureCache()
        {
            return *mp_txtcache;
        }

        inline FontStore& GetFontStore()
        {
            return *mp_fonts;
        }

        int MainLoop();
        void Terminate();
        inline void EnableGUI() { m_render_gui = true; }
        inline void DisableGUI() { m_render_gui = false; }

        std::unique_ptr<Scene> PopScene();
        void PushScene(std::unique_ptr<Scene> p_scene);

    private:
        sf::RenderWindow* mp_window;
        Configuration* mp_config;
        Pathmap* mp_pathmap;
        TextureCache* mp_txtcache;
        FontStore* mp_fonts;
        nk_context* mp_gui;
        nk_user_font* mp_gui_font;
        bool m_terminate;
        bool m_render_gui;
        std::stack<std::unique_ptr<Scene>> m_scene_stack;

        void OpenWindow();
        void InitGUI();
        void CleanupGUI();
        void DrawGUI();
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
