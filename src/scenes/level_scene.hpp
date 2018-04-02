#ifndef TSC_LEVEL_SCENE_HPP
#define TSC_LEVEL_SCENE_HPP
#include <SFML/Graphics.hpp>
#include "scene.hpp"
#include "../ground.hpp"

namespace TSC {

    class LevelScene: public Scene
    {
    public:
        LevelScene();
        virtual ~LevelScene();

        virtual void Update(const sf::RenderWindow& stage);
        virtual void Draw(sf::RenderWindow& stage) const;
    private:
        sf::View m_view;
        Ground m_ground;
    };

}

#endif /* TSC_LEVEL_SCENE_HPP */
