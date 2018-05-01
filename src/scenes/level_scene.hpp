#ifndef TSC_LEVEL_SCENE_HPP
#define TSC_LEVEL_SCENE_HPP
#include <SFML/Graphics.hpp>
#include "scene.hpp"
#include "../level.hpp"

namespace TSC {

    class LevelScene: public Scene
    {
    public:
        LevelScene();
        virtual ~LevelScene();

        virtual void ProcessEvent(sf::Event& event);
        virtual void Update(const sf::RenderTarget& stage);
        virtual void Draw(sf::RenderTarget& stage) const;
    private:
        sf::View m_view;
        Level m_level;
    };

}

#endif /* TSC_LEVEL_SCENE_HPP */
