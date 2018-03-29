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

/* This file includes the nuklear GUI toolkit in "implementation mode", see
 * the nuklear docs. The actual configuration of nuklear happens in gui.hpp
 * so that it's the same each time nuklear is included. */

#define NK_IMPLEMENTATION
#include "gui.hpp"
#include "application.hpp"
#include "font_store.hpp"
#include <SFML/Graphics.hpp>

using namespace TSC;

/**
 * Converts the given nuklear color to an SFML color.
 */
inline sf::Color NKColor2SFColor(struct nk_color color)
{
    return sf::Color(color.r, color.g, color.b, color.a);
}


/**
 * Calculates the width of the given text in rendered form when the
 * normal GUI font (NormalFont member) is used.
 */
float TSC::CalculateGUIFontWidth(nk_handle, float, const char* text, int textlen)
{
    sf::Text sftext(std::string(text, textlen), gp_app->GetFontStore().NormalFont, TSC::GUI_FONT_SIZE);
    return sftext.getLocalBounds().width;
}

/**
 * Draws the GUI using nuklear. This function translates each command
 * in the nuklear rendering pipeline to the corresponding SFML
 * commands, if any.
 */
void TSC::DrawNKGUI(struct nk_context* p_ctx, sf::RenderWindow* p_window)
{
    const struct nk_command *p_cmd = nullptr;

    nk_foreach(p_cmd, p_ctx) {
        sf::Color color;
        switch (p_cmd->type) {
        case NK_COMMAND_NOP:
            break;
        case NK_COMMAND_SCISSOR:
            // FIXME: SFML does not support scissoring (clipping of the drawing area).
            break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line* l = (const struct nk_command_line*) p_cmd;
            color = NKColor2SFColor(l->color);
            sf::Vertex line[] = {sf::Vertex(sf::Vector2f(l->begin.x, l->begin.y), color),
                                 sf::Vertex(sf::Vector2f(l->end.x, l->end.y), color)};
            p_window->draw(line, 2, sf::Lines);
            // TODO: Line thickness

        } break;
        case NK_COMMAND_POLYLINE: {
            // "Polyline" = Multiple lines after one another joined
            const struct nk_command_polyline* p = (const struct nk_command_polyline*) p_cmd;
            sf::VertexArray polyline(sf::LineStrip, p->point_count);
            for (int i=0; i < p->point_count; i++) {
                polyline[i].position = sf::Vector2f(p->points[i].x, p->points[i].y);
                polyline[i].color = NKColor2SFColor(p->color);
            }
            // TODO: Line thickness
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect* r = (const struct nk_command_rect*) p_cmd;
            sf::RectangleShape rect;
            rect.setPosition(r->x, r->y);
            rect.setSize(sf::Vector2f(r->w, r->h));
            rect.setOutlineColor(NKColor2SFColor(r->color));
            rect.setOutlineThickness(r->line_thickness);
            // TODO: Round corners: r->rounding
            p_window->draw(rect);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled* r = (const struct nk_command_rect_filled*) p_cmd;
            sf::RectangleShape rect;
            rect.setPosition(r->x, r->y);
            rect.setSize(sf::Vector2f(r->w, r->h));
            rect.setFillColor(NKColor2SFColor(r->color));
            // TODO: Round corners: r->rounding
            p_window->draw(rect);
        } break;
        case NK_COMMAND_CIRCLE: {
            // nuklear describes a circle as top-left corner plus width/height as if it were a rectangle
            const struct nk_command_circle* c = (const struct nk_command_circle*) p_cmd;
            float radius = static_cast<float>(c->w)/2.0f;
            sf::CircleShape circle;
            circle.setPosition(c->x + radius, c->y + radius);
            circle.setRadius(radius);
            circle.setOutlineColor(NKColor2SFColor(c->color));
            circle.setOutlineThickness(c->line_thickness);
            p_window->draw(circle);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            // nuklear describes a circle as top-left corner plus width/height as if it were a rectangle
            const struct nk_command_circle_filled* c = (const struct nk_command_circle_filled*) p_cmd;
            float radius = static_cast<float>(c->w)/2.0f;
            sf::CircleShape circle;
            circle.setPosition(c->x + radius, c->y + radius);
            circle.setRadius(radius);
            circle.setFillColor(NKColor2SFColor(c->color));
            p_window->draw(circle);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle* t = (const struct nk_command_triangle*) p_cmd;
            sf::ConvexShape triangle(3);
            triangle.setPoint(0, sf::Vector2f(t->a.x, t->a.y));
            triangle.setPoint(1, sf::Vector2f(t->b.x, t->b.y));
            triangle.setPoint(2, sf::Vector2f(t->c.x, t->c.y));
            triangle.setOutlineColor(NKColor2SFColor(t->color));
            triangle.setOutlineThickness(t->line_thickness);
            p_window->draw(triangle);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled* t = (const struct nk_command_triangle_filled*) p_cmd;
            sf::ConvexShape triangle(3);
            triangle.setPoint(0, sf::Vector2f(t->a.x, t->a.y));
            triangle.setPoint(1, sf::Vector2f(t->b.x, t->b.y));
            triangle.setPoint(2, sf::Vector2f(t->c.x, t->c.y));
            triangle.setFillColor(NKColor2SFColor(t->color));
            p_window->draw(triangle);
        } break;
        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon* p = (const struct nk_command_polygon*) p_cmd;
            sf::ConvexShape polygon(p->point_count);
            for (int i=0; i < p->point_count; i++) {
                polygon.setPoint(i, sf::Vector2f(p->points[i].x, p->points[i].y));
            }
            polygon.setOutlineColor(NKColor2SFColor(p->color));
            polygon.setOutlineThickness(p->line_thickness);
            p_window->draw(polygon);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon* p = (const struct nk_command_polygon*) p_cmd;
            sf::ConvexShape polygon(p->point_count);
            for (int i=0; i < p->point_count; i++) {
                polygon.setPoint(i, sf::Vector2f(p->points[i].x, p->points[i].y));
            }
            polygon.setFillColor(NKColor2SFColor(p->color));
            p_window->draw(polygon);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text* t = (const struct nk_command_text*) p_cmd;
            sf::Font* p_font = (sf::Font*) t->font->userdata.ptr;
            sf::Text text(std::string(t->string), *p_font, TSC::GUI_FONT_SIZE);
            text.setFillColor(NKColor2SFColor(t->foreground));
            text.setPosition(sf::Vector2f(t->x, t->y));
            p_window->draw(text);
        } break;
        case NK_COMMAND_CURVE:
            // FIXME: SFML does not support splines
            break;
        case NK_COMMAND_ARC:
            // FIXME: SFML does not support bezier arcs
            break;
        case NK_COMMAND_RECT_MULTI_COLOR:
            // TODO: This is possible, but since nuklear gives the desired color of the edges
            // of the rectangle rather than of the corners, it requires calculating the corner
            // colors, as those are the ones that can be set on an SFML Vertex.
            break;
        case NK_COMMAND_IMAGE:
            // Not documented. No idea what to do here.
            break;
        case NK_COMMAND_ARC_FILLED:
            // FIXME: SFML does not support bezier arcs
            break;
        case NK_COMMAND_CUSTOM:
            // Do nothing
            break;
        } // No default clause so the compiler can warn about missing values
    }

    nk_clear(p_ctx);
}
