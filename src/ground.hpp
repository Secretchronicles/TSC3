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

#ifndef TSC_GROUND_HPP
#define TSC_GROUND_HPP
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace TSC {

    /**
     * A field is the description of where to use a specific tile of
     * a tileset. It consists of the x/y coordinate of the field in the
     * level and the tile ID, which gives the index of the tile in the tileset,
     * starting from zero (0) for the top-left tile, increasing towards the
     * bottom-right tile of the tileset.
     */
    struct Field
    {
        Field()
            : x(0.0f), y(0.0f), tileid(0) {}
        Field(float xx, float yy, int t)
            : x(xx), y(yy), tileid(t) {}

        float x;
        float y;
        int tileid;
    };

    /**
     * The Ground is an SFML-like entity to draw the level ground from a
     * tileset. It uses just a single, large vertex array for the entire
     * level ground, thus rendering a Ground object is pretty fast despite
     * of its usually large extends. The downside of it is that you're
     * restricted to construct your Ground object from a single tileset.
     * This is actually good for level consistency, but if you really need
     * a second tileset, you can still instanciate a second Ground object.
     *
     * The tileset consists of *tiles* as drawn in your favourite graphics
     * programme to the PNG file. The tileset has columns and rows; how many,
     * is defined via the tileset metadata XML file. The width and height of
     * the image in pixels have to be evenly dividable through the number
     * of columns and rows, respectively, otherwise you will receive undesired
     * drawing errors.
     *
     * The ground then consists of *fields* (class Field), which describe where
     * to place a copy of which tile. This list of fields, along with the
     * desired tileset to draw them from, constitute the arguments of the Ground()
     * constructor. It is not the Ground class' job to parse the list of
     * fields from the level XML format; that's left for the level loader. This
     * is proper task division and allows to construct a Ground object from
     * memory directly if required (could be useful for scripting).
     *
     * Collision information for the ground is read from the tileset metadata
     * XML as well.
     */
    class Ground: public sf::Drawable, public sf::Transformable
    {
    public:
        Ground(const std::string& tileset, const std::vector<Field>& fields);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void LoadSettingsFile(const std::string& path);
        void ReadVertices(const std::vector<Field>& fields, const sf::Image& img);

        int m_rows;
        int m_cols;
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
        std::vector<sf::FloatRect> m_colrects;
    };

}

#endif /* TSC_GROUND_HPP */
