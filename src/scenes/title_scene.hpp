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

#ifndef TSC_TITLE_SCENE_HPP
#define TSC_TITLE_SCENE_HPP
#include "scene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace TSC {

    class TitleScene: public Scene {
    public:
        TitleScene();
        virtual ~TitleScene();

        virtual void ProcessEvent(sf::Event& event);
        virtual void DoGUI(const sf::RenderTarget& stage);
        virtual void Update(const sf::RenderTarget& stage);
        virtual void Draw(sf::RenderTarget& stage) const;

        sf::Sprite m_background;
    };

}

#endif /* TSC_TITLE_SCENE_HPP */
