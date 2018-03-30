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
#include "pathmap.hpp"
#include <pathie/path.hpp>
#include <SFML/Graphics.hpp>

using namespace TSC;
using Pathie::Path;

// Global GUI information.
static nk_context s_gui_context;
static nk_user_font s_gui_font;
static bool s_gui_enabled = true;

// "extern" declarations
sf::Font GUI::NormalFont;
sf::Font GUI::BoldFont;
sf::Font GUI::MonospaceFont;
sf::Font GUI::MonospaceBoldFont;

/**
 * Converts the given nuklear color to an SFML color.
 */
static inline sf::Color NKColor2SFColor(struct nk_color color)
{
    return sf::Color(color.r, color.g, color.b, color.a);
}

// Converts the given SFML mouse button to a nuklear mouse button.
static inline enum nk_buttons SFButton2NKButton(sf::Mouse::Button button)
{
    switch (button) {
    case sf::Mouse::Button::Left:
        return NK_BUTTON_LEFT;
    case sf::Mouse::Button::Right:
        return NK_BUTTON_RIGHT;
    case sf::Mouse::Button::Middle:
        return NK_BUTTON_MIDDLE;
    default:
        return NK_BUTTON_LEFT;
    }
}


// Converts the given SFML key into a nuklear key.
static enum nk_keys SFKey2NKKey(sf::Keyboard::Key key)
{
    switch (key) {
    case sf::Keyboard::LControl: // fallthrough
    case sf::Keyboard::RControl:
        return NK_KEY_CTRL;
    case sf::Keyboard::Delete:
        return NK_KEY_DEL;
    case sf::Keyboard::Return:
        return NK_KEY_ENTER;
    case sf::Keyboard::Tab:
        return NK_KEY_TAB;
    case sf::Keyboard::BackSpace:
        return NK_KEY_BACKSPACE;
    case sf::Keyboard::Up:
        return NK_KEY_UP;
    case sf::Keyboard::Down:
        return NK_KEY_DOWN;
    case sf::Keyboard::Left:
        return NK_KEY_LEFT;
    case sf::Keyboard::Right:
        return NK_KEY_RIGHT;
    case sf::Keyboard::Insert:
        return NK_KEY_TEXT_INSERT_MODE;
    case sf::Keyboard::Home:
        return NK_KEY_TEXT_LINE_START;
    case sf::Keyboard::End:
        return NK_KEY_TEXT_LINE_END;
    case sf::Keyboard::PageUp:
        return NK_KEY_SCROLL_UP;
    case sf::Keyboard::PageDown:
        return NK_KEY_SCROLL_DOWN;
    default:
        return NK_KEY_NONE;
    }
}

/**
 * Calculates the width of the given text in rendered form when the
 * normal GUI font (NormalFont member) is used.
 */
static float CalculateGUIFontWidth(nk_handle, float, const char* text, int textlen)
{
    sf::Text sftext(std::string(text, textlen), GUI::NormalFont, TSC::GUI::NORMAL_FONT_SIZE);
    return sftext.getLocalBounds().width;
}

/**
 * Initialises the GUI system by loading everything needed from the disk.
 * Do not use any GUI functionality before you have called this function.
 * This function must be matched by a corresponding Cleanup() call.
 *
 * \see Cleanup()
 */
void GUI::Init()
{
    // Load the fonts from disk
    Path dir = Pathmap::GetFontPath();
    NormalFont.loadFromFile(dir.join("DejaVuSans.ttf").utf8_str());
    BoldFont.loadFromFile(dir.join("DejaVuSans-Bold.ttf").utf8_str());
    MonospaceFont.loadFromFile(dir.join("DejaVuSansMono.ttf").utf8_str());
    MonospaceBoldFont.loadFromFile(dir.join("DejaVuSansMono-Bold.ttf").utf8_str());

    // Initialise "nuklear" GUI toolkit
    s_gui_font.userdata.ptr = &NormalFont;
    s_gui_font.height = NORMAL_FONT_SIZE;
    s_gui_font.width = CalculateGUIFontWidth;
    nk_init_default(&s_gui_context, &s_gui_font);
}

/**
 * Frees memory allocated with Init(). Call this at the end of the programme.
 *
 * \see Init()
 */
void GUI::Cleanup()
{
    nk_free(&s_gui_context);
}

/**
 * Returns the pointer to the global "nuklear" GUI context.
 * This points to static memory. Do not free this. This method
 * is only intended to give you access to it so that you can use
 * nuklear's GUI construction functions.
 */
nk_context* GUI::Get()
{
    return &s_gui_context;
}

/**
 * Forward the given event to the GUI system. When processing the events
 * in the main loop, call this once for each event, and before Draw().
 *
 * \see Draw()
 */
void GUI::ProcessEvent(sf::Event& event)
{
    if (!s_gui_enabled)
        return;

    nk_input_begin(&s_gui_context);

    static struct nk_vec2 scrollvec;
    switch (event.type) {
    case sf::Event::MouseMoved:
        nk_input_motion(&s_gui_context, event.mouseMove.x, event.mouseMove.y);
        break;
    case sf::Event::MouseButtonPressed:
        nk_input_button(&s_gui_context, SFButton2NKButton(event.mouseButton.button), event.mouseButton.x, event.mouseButton.y, 1);
        break;
    case sf::Event::MouseButtonReleased:
        nk_input_button(&s_gui_context, SFButton2NKButton(event.mouseButton.button), event.mouseButton.x, event.mouseButton.y, 0);
        break;
    case sf::Event::MouseWheelScrolled:
        scrollvec.x = scrollvec.y = 0;

        if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::HorizontalWheel)
            scrollvec.x = event.mouseWheelScroll.delta;
        else // normal vertical wheel
            scrollvec.y = event.mouseWheelScroll.delta;

        nk_input_scroll(&s_gui_context, scrollvec);
        break;
    case sf::Event::KeyPressed:
        nk_input_key(&s_gui_context, SFKey2NKKey(event.key.code), 1);
        break;
    case sf::Event::KeyReleased:
        nk_input_key(&s_gui_context, SFKey2NKKey(event.key.code), 0);
        break;
    case sf::Event::TextEntered:
        nk_input_unicode(&s_gui_context, event.text.unicode);
        break;
    default:
        // Ignore
        break;
    }

    nk_input_end(&s_gui_context);
}

/**
 * Draws the GUI using nuklear. This function translates each command
 * in the nuklear rendering pipeline to the corresponding SFML
 * commands, if any.
 *
 * Call this once in the mainloop, after ProcessEvents().
 *
 * \see ProcessEvent()
 */
void GUI::Draw(sf::RenderWindow& window)
{
    if (!s_gui_enabled) {
        // Frame clearing is still required, otherwise nuklear gets confused.
        nk_clear(&s_gui_context);
        return;
    }

    const struct nk_command *p_cmd = nullptr;

    nk_foreach(p_cmd, &s_gui_context) {
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
            window.draw(line, 2, sf::Lines);
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
            window.draw(polyline);
            // TODO: Line thickness
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect* r = (const struct nk_command_rect*) p_cmd;
            sf::RectangleShape rect;
            rect.setPosition(r->x, r->y);
            rect.setSize(sf::Vector2f(r->w, r->h));
            rect.setOutlineColor(NKColor2SFColor(r->color));
            rect.setOutlineThickness(r->line_thickness);
            rect.setFillColor(sf::Color::Transparent);
            // TODO: Round corners: r->rounding
            window.draw(rect);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled* r = (const struct nk_command_rect_filled*) p_cmd;
            sf::RectangleShape rect;
            rect.setPosition(r->x, r->y);
            rect.setSize(sf::Vector2f(r->w, r->h));
            rect.setFillColor(NKColor2SFColor(r->color));
            // TODO: Round corners: r->rounding
            window.draw(rect);
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
            circle.setFillColor(sf::Color::Transparent);
            window.draw(circle);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled* c = (const struct nk_command_circle_filled*) p_cmd;
            float radius = static_cast<float>(c->w)/2.0f;
            sf::CircleShape circle;
            circle.setPosition(c->x, c->y);
            circle.setRadius(radius);
            circle.setFillColor(NKColor2SFColor(c->color));
            window.draw(circle);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle* t = (const struct nk_command_triangle*) p_cmd;
            sf::ConvexShape triangle(3);
            triangle.setPoint(0, sf::Vector2f(t->a.x, t->a.y));
            triangle.setPoint(1, sf::Vector2f(t->b.x, t->b.y));
            triangle.setPoint(2, sf::Vector2f(t->c.x, t->c.y));
            triangle.setOutlineColor(NKColor2SFColor(t->color));
            triangle.setOutlineThickness(t->line_thickness);
            triangle.setFillColor(sf::Color::Transparent);
            window.draw(triangle);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled* t = (const struct nk_command_triangle_filled*) p_cmd;
            sf::ConvexShape triangle(3);
            triangle.setPoint(0, sf::Vector2f(t->a.x, t->a.y));
            triangle.setPoint(1, sf::Vector2f(t->b.x, t->b.y));
            triangle.setPoint(2, sf::Vector2f(t->c.x, t->c.y));
            triangle.setFillColor(NKColor2SFColor(t->color));
            window.draw(triangle);
        } break;
        case NK_COMMAND_POLYGON: {
            const struct nk_command_polygon* p = (const struct nk_command_polygon*) p_cmd;
            sf::ConvexShape polygon(p->point_count);
            for (int i=0; i < p->point_count; i++) {
                polygon.setPoint(i, sf::Vector2f(p->points[i].x, p->points[i].y));
            }
            polygon.setOutlineColor(NKColor2SFColor(p->color));
            polygon.setOutlineThickness(p->line_thickness);
            polygon.setFillColor(sf::Color::Transparent);
            window.draw(polygon);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            const struct nk_command_polygon* p = (const struct nk_command_polygon*) p_cmd;
            sf::ConvexShape polygon(p->point_count);
            for (int i=0; i < p->point_count; i++) {
                polygon.setPoint(i, sf::Vector2f(p->points[i].x, p->points[i].y));
            }
            polygon.setFillColor(NKColor2SFColor(p->color));
            window.draw(polygon);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text* t = (const struct nk_command_text*) p_cmd;
            sf::Font* p_font = (sf::Font*) t->font->userdata.ptr;
            std::string utf8str(t->string);
            sf::Text text(sf::String::fromUtf8(utf8str.begin(), utf8str.end()), *p_font, TSC::GUI::NORMAL_FONT_SIZE);
            text.setFillColor(NKColor2SFColor(t->foreground));
            text.setPosition(sf::Vector2f(t->x, t->y));
            window.draw(text);
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

    // Tell nuklear GUI drawing is over for this frame.
    nk_clear(&s_gui_context);
}

/// Enable the GUI after it was Disable()d.
void GUI::Enable()
{
    s_gui_enabled = true;
}

/**
 * Disable the GUI. You can enable it with Enable() again.
 * Disabling the GUI causes the ProcessEvent() and Draw()
 * functions to return early instead of actually executing
 * their work. This can be used to improve performance where
 * GUI information is not needed.
 */
void GUI::Disable()
{
    s_gui_enabled = false;
}

/// Toggle whether the GUI is enabled.
void GUI::Toggle()
{
    s_gui_enabled = !s_gui_enabled;
}

/// Checks whether the GUI is currently enabled.
bool GUI::IsEnabled()
{
    return s_gui_enabled;
}
