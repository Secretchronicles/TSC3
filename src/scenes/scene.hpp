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

#ifndef TSC_SCENE_HPP
#define TSC_SCENE_HPP

// forward-declare
namespace sf {
    class RenderWindow;
    class Event;
}

namespace TSC {

    /**
     * Base class for all scenes. When creating a new scene, override the virtual
     * methods provided in this class as you need.
     *
     * ### Call Order of Methods ###
     *
     * When the main loop iterates, it calls the currently active
     * scene's methods; see "The Scene Stack" below on how the
     * active scene is determined. The methods are called in the
     * following order:
     *
     * 1. ProcessEvent(), once for each event received from the user.
     * 2. DoGUI()
     * 3. Update()
     * 4. Draw()
     * 5. LateUpdate()
     *
     * At minimum, you have to override the Update() and Draw() functions.
     * The former's purpose is to update scene's internal state, e.g.
     * move enemies around. The latter's purpose is to then draw all
     * the scene's entities in the state Update() left them in. That's
     * also why Update() only receives a const reference to the SFML
     * window rather than a normal one as Draw() does -- you're not supposed
     * to mess with the drawing in Update().
     *
     * DoGUI() is actually a special-purpose Update() function. The GUI
     * toolkit in use, nuklear, demands that GUI drawing is done as early
     * in the main loop as possible (but after event processing) which
     * explains the order in which in was inserted above. Override DoGUI()
     * if you need to display windows to the user; do not do that
     * in Update().
     *
     * ProcessEvent() receives user input. Use it to move around the hero
     * or similar.
     *
     * LateUpdate() is called just before the main loop ends an
     * iteration, which especially means it's executed after Draw()ing
     * everything to the window. There's little use for this function
     * and you should really not be using it at all. Do your update
     * work in Update().
     *
     * In general, for all of these functions except for Update() and
     * Draw() (which are pure virtual functions), you want to call the
     * parent class' method as the first thing you do when you
     * override them.
     *
     * ### The Scene Stack ###
     *
     * TSC is following a scene model, where each "happening" on the
     * screen is controlled by a specific scene. So there's a title
     * screen scene, a level scene, a menu scene, etc. All currently
     * active scenes form the *scene stack*, which is managed by the
     * Application class. The topmost scene in this stack is the
     * currently active scene, which receives user input, is updated
     * and drawn.
     *
     * The regular way to switch scenes is to simply push the next scene onto
     * the scene stack by calling Application::PushScene(). Once the main loop
     * reaches its next iteration, the new scene will be played as it then
     * constitutes the top of the scene stack. Once that scene calls Finish(),
     * it is marked as finished and the main loop will pop if off the scene
     * stack in its next iteration. As a result, the original scene, which lies
     * below the finished scene, comes to life again. This way, it's easily possible
     * to have a menu scene that returns e.g. to the level scene once the menu is
     * closed.
     *
     * As a side effect, the bottom-most scene in the scene stack is always the title
     * scene. If this scene finishes (the user uses the "quit" button on the title
     * scene), the scene stack is left empty. And empy scene stack will cause the
     * main loop to terminate and thus end the programme.
     *
     * A problem arises if you want to terminate the current scene permanently and
     * simulateneously want to switch to a new scene. Usually this can be avoided
     * by pushing two scenes on the stack directly after one another, first the
     * subsequent scene and then the scene that terminates permanently. That way,
     * if the problematic scene terminates, below it will be the subsequent scene.
     *
     * The problem with this approach is that you don't always know the subsequent
     * scene when you push the problematic scene onto the stack. To work around this,
     * you can't rely on the Finish() mechanism anymore, but have to pop the scene
     * off the scene stack manually. When you want to terminate your problematic
     * scene, do as follows:
     *
     *  1. In your scene's LateUpdate() function, pop the problematic scene off
     *     the stack using Application::PopScene(). Ensure you store the return
     *     value of that method until your problematic scene has finished,
     *     otherwise your running instance (`this`) may be deleted on the return
     *     of PopScene(), which is inevitably going to segfault the programme
     *     (that is, `this` and the return value of PopScene() are the same pointer).
     *  2. Push the subsequent scene onto the stack using Application::PushScene().
     *  3. Do any cleanup your problematic scene requires.
     *  4. Let the return value of PopScene(), which you captured in step 1, run out
     *     of scope. As it's a std::unique_ptr, this will cause C++ to free the instance.
     *  5. On the next iteration of the mainloop, the subsequent scene is on top of the stack
     *     and will run as expected.
     *
     * This is the only legitimate use of LateUpdate().
     */
    class Scene {
    public:
        Scene();
        virtual ~Scene() {}

        /// Override this function if you need user input
        /// outside the GUI. It gets passed one event at a time.
        virtual void ProcessEvent(sf::Event& event);

        /// Override this function if you want to show GUI windows
        /// to the user. You can use nuklear's GUI construction
        /// functions in this method.
        virtual void DoGUI() {}

        /**
         * Update() shall update all logic in the scene, but not
         * draw anything onto the screen. Calling Finish() from this
         * method causes the main loop to pop the scene from the
         * scene stack at the beginning of the next frame.
         */
        virtual void Update(const sf::RenderWindow& stage) = 0;
        /// Draw() shall draw the updated scene onto the screen.
        /// Game logic updates should be in Update().
        virtual void Draw(sf::RenderWindow& stage) const = 0;

        /// This function is called at the end of the main loop.
        /// You should really not use it. See the class docs
        /// for the probably only legitimate use of it. it does
        /// nothing by default.
        virtual void LateUpdate() {}

        void Finish();
        bool HasFinished() const;
    private:
        bool m_finish;
    };

}

#endif /* TSC_SCENE_HPP */
