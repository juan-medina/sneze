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
#include <cstdint>

#include <entt/fwd.hpp>

#include "../components/geometry.hpp"
#include "../device/keyboard.hpp"
#include "../device/mouse.hpp"

namespace sneze {
class world;

//! events namespace.
namespace events {

//! event base class.
struct event {
    //! the current world.
    class world *world; // cppcheck-suppress unusedStructMember
};

//! event that indicates that the application is closing.
struct application_want_closing: public event {};

//! event that indicates that we want to toggle fullscreen.
struct toggle_fullscreen: public event {};

//! key base event.
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
struct key_event: public event, public keyboard::key_modifier {};

//! key down event.
struct key_down: key_event {};

//! key up event.
struct key_up: key_event {};

//! window resized event.
struct window_resized: public event {
    //! the new size of the window.
    components::size window; // cppcheck-suppress unusedStructMember
    /**
     * @brief new size of the client area, may be different from the original size.
     * @see config::logical
     */
    components::rect logical; // cppcheck-suppress unusedStructMember
};

/**
 * @brief event that indicates that we want to add a component to an entity.
 * @tparam ComponentType the type of the component to add.
 */
template<typename ComponentType>
struct add_component: public event {
    //! the entity id
    entt::entity entity;
    //! the component to add
    ComponentType component;
};

//! mouse moved event.
struct mouse_moved: public event {
    //! the new position of the mouse.
    components::position point; // cppcheck-suppress unusedStructMember
};

//! mouse button base event.
struct mouse_button: public event {
    //! the button that was used.
    mouse::button button; // cppcheck-suppress unusedStructMember
    //! the position of the mouse.
    components::position point; // cppcheck-suppress unusedStructMember
};

//! mouse button down event.
struct mouse_button_down: public mouse_button {};

//! mouse button up event.
struct mouse_button_up: public mouse_button {};

} // namespace events

} // namespace sneze
