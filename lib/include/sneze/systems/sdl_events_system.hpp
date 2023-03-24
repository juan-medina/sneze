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

#include <cinttypes>
#include <utility>

#include "../events/events.hpp"

#include "system.hpp"

namespace sneze {

class render;

/**
 * @brief sdl events system
 *
 * This system will handle all the events from the sdl library
 *
 */
class sdl_events_system: public system {
public:
    /**
     * @brief Construct a new sdl events system object
     *
     * @param render the render object
     */
    explicit sdl_events_system(std::shared_ptr<render> render): render_{std::move(render)} {};

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
    //! the render object
    std::shared_ptr<sneze::render> render_;

    //! convert sdl mouse button to sneze mouse button
    static auto sdl_mouse_button_to_sneze(uint8_t button) -> mouse::button;
};

} // namespace sneze