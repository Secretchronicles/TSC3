#include "level_scene.hpp"

using namespace TSC;
using namespace std;

LevelScene::LevelScene()
    : m_level("test_level.tsc3lvl")
{
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
    m_level.Update();
}

void LevelScene::Draw(sf::RenderTarget& stage) const
{
    m_level.Draw(stage);
}
