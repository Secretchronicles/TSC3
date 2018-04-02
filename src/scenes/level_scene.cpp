#include "level_scene.hpp"

using namespace TSC;
using namespace std;

LevelScene::LevelScene()
{
    vector<Field> fields;
    fields.emplace_back(0, 0, 0);
    fields.emplace_back(256, 0, 1);
    fields.emplace_back(512, 0, 2);
    fields.emplace_back(0, 256, 3);
    fields.emplace_back(256, 256, 4);
    fields.emplace_back(512, 256, 5);

    m_ground.reset("green.png", fields);
    m_ground.setPosition(100, 500);
}

LevelScene::~LevelScene()
{
}

void LevelScene::Update(const sf::RenderWindow&)
{
}

void LevelScene::Draw(sf::RenderWindow& stage) const
{
    stage.draw(m_ground);
}
