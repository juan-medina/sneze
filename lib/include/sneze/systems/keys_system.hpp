/****************************************************************************
MIT License

Copyright (c) 2023 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

#pragma once

#include "../events/events.hpp"

#include "system.hpp"

namespace sneze {

/**
 * @brief keys system
 *
 * system that handles the keyboard events
 */
class keys_system: public system {
public:
    /**
     * @brief Construct a new keys system object
     * @param exit the key modifier to exit the game
     * @param toggle_fullscreen the key modifier to toggle fullscreen
     */
    keys_system(const keyboard::key_modifier &exit, const keyboard::key_modifier &toggle_fullscreen)
        : exit_(exit), toggle_full_screen_(toggle_fullscreen) {}

    /**
     * @brief initialize the system
     * @param world the world that owns this system
     */
    void init(world *world) override;

    /**
     * @brief shutdown the system
     * @param world the world that owns this system
     */
    void end(world *world) override;

    /**
     * @brief update the system
     * @param world the world that owns this system
     */
    void update(world *world) override;

private:
    //! handle the key up event
    void key_up(const events::key_up &event);

    //! exit key and modifier
    keyboard::key_modifier exit_;

    //! toggle fullscreen key and modifier
    keyboard::key_modifier toggle_full_screen_;
};

} // namespace sneze