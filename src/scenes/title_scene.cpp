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

#include "title_scene.hpp"
#include "../application.hpp"
#include "../texture_cache.hpp"

using namespace std;
using namespace TSC;

TitleScene::TitleScene()
{
    m_background.setTexture(gp_app->GetTextureCache().Get("misc/title.png"));
    m_background.setPosition(sf::Vector2f(0, 0));
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Update()
{
    return true;
}

void TitleScene::Draw(sf::RenderWindow* p_stage) const
{
    p_stage->draw(m_background);
}
