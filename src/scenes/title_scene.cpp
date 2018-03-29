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
#include "../pathmap.hpp"
#include "../gui.hpp"
#include "../i18n.hpp"

using namespace std;
using namespace TSC;

TitleScene::TitleScene()
{
    m_background.setTexture(TextureCache::Get("misc/title.png"));
    m_background.setPosition(sf::Vector2f(0, 0));

    m_music.openFromFile((Pathmap::GetMusicPath() / "title.ogg").utf8_str());
    m_music.setLoop(true);
    m_music.play();
}

TitleScene::~TitleScene()
{
    m_music.stop();
}

bool TitleScene::Update()
{
    nk_context* p_ctx = gp_app->GetGUI();

    if (nk_begin(p_ctx, _("Title Menu"), nk_rect(500, 300, 200, 400), NK_WINDOW_BORDER|NK_WINDOW_NO_INPUT)) {
        nk_layout_row_dynamic(p_ctx, 50, 1);
        if (nk_button_label(p_ctx, _("Start"))) {
            // TODO: Event handling
        }
        if (nk_button_label(p_ctx, _("Levels"))) {
            // TODO: Event handling
        }
        if (nk_button_label(p_ctx, _("Settings"))) {
            // TODO: Event handling
        }
        if (nk_button_label(p_ctx, _("Quit"))) {
            // TODO: Event handling
        }
    }
    nk_end(p_ctx);

    return true;
}

void TitleScene::Draw(sf::RenderWindow* p_stage) const
{
    p_stage->draw(m_background);
}
