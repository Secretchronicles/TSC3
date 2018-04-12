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
#include "level_scene.hpp"
#include "../texture_cache.hpp"
#include "../pathmap.hpp"
#include "../audio.hpp"
#include "../gui.hpp"
#include "../application.hpp"
#include "../i18n.hpp"

using namespace std;
using namespace TSC;

TitleScene::TitleScene()
{
    m_background.setTexture(TextureCache::Get("misc/title.png"));
    m_background.setPosition(sf::Vector2f(0, 0));
    m_background.setScale(Application::Instance()->GetGlobalScale(), Application::Instance()->GetGlobalScale());

    Audio::PlayMusic("title.ogg");
}

TitleScene::~TitleScene()
{
}

void TitleScene::ProcessEvent(sf::Event& event)
{
    Scene::ProcessEvent(event);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        Finish();
    }
}

void TitleScene::DoGUI(const sf::RenderTarget& stage)
{
    Scene::DoGUI(stage);

    nk_context* p_ctx = GUI::Get();

    int x = (stage.getSize().x - 200) / 2;
    int y = (stage.getSize().y - 400) / 2;

    if (nk_begin(p_ctx, _("Title Menu"), nk_rect(x, y, 200, 400), NK_WINDOW_BORDER|NK_WINDOW_NO_INPUT)) {
        nk_layout_row_dynamic(p_ctx, 50, 1);
        if (nk_button_label(p_ctx, _("Start"))) {
            Application::Instance()->PushScene(unique_ptr<LevelScene>(new LevelScene()));
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
}

void TitleScene::Update(const sf::RenderTarget&)
{
}

void TitleScene::Draw(sf::RenderTarget& stage) const
{
    stage.draw(m_background);
}
