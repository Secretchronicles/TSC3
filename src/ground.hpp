#ifndef TSC_GROUND_HPP
#define TSC_GROUND_HPP
#include <string>
#include <SFML/Graphics.hpp>

namespace TSC {

    class Ground: public sf::Drawable, public sf::Transformable
    {
    public:
        Ground(const std::string& tileset);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
    };

}

#endif /* TSC_GROUND_HPP */
