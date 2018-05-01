#ifndef TSC_LEVEL_HPP
#define TSC_LEVEL_HPP
#include <string>
#include <vector>
#include "ground.hpp"

namespace TSC {

    class LevelLoader;

    class Level
    {
    friend class LevelLoader;
    public:
        Level(const std::string& relfilename);
        ~Level();

        void Update();
        void Draw(sf::RenderTarget& stage) const;
    private:
        int m_width;
        int m_height;
        int m_fixed_cam_speed;
        std::string m_music;
        std::vector<Ground> m_grounds;
    };

}

#endif /* TSC_LEVEL_HPP */
