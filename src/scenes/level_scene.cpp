#include "level_scene.hpp"

using namespace TSC;
using namespace std;

LevelScene::LevelScene()
{
    vector<Field> fields;

    for (int x=0; x < 1920; x += 64) {
        fields.emplace_back(x, 1016, 1);
    }

    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(0, y, 3);
    }
    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(64, y, 5);
    }

    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(1152, y, 3);
    }
    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(1216, y, 5);
    }

    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(1792, y, 3);
    }
    for (int y=952; y >= -64; y -= 64) {
        fields.emplace_back(1856, y, 5);
    }

    m_ground.reset("green_3.png", fields);
    m_ground.setPosition(0, 0);
}

LevelScene::~LevelScene()
{
}

void LevelScene::ProcessEvent(sf::Event& event)
{
    Scene::ProcessEvent(event);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        Finish();
    }
}

void LevelScene::Update(const sf::RenderTarget&)
{
}

void LevelScene::Draw(sf::RenderTarget& stage) const
{
    stage.draw(m_ground);
}
