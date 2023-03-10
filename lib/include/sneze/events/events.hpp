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

namespace events {

struct event {
    class world *world; // cppcheck-suppress unusedStructMember
};

struct application_want_closing: public event {};

struct toggle_fullscreen: public event {};

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
struct key_event: public event, public keyboard::key_modifier {};

struct key_down: key_event {};

struct key_up: key_event {};

struct window_resized: public event {
    components::size window;  // cppcheck-suppress unusedStructMember
    components::rect logical; // cppcheck-suppress unusedStructMember
};

template<typename ComponentType>
struct add_component: public event {
    entt::entity entity;
    ComponentType component;
};

struct mouse_moved: public event {
    components::position point; // cppcheck-suppress unusedStructMember
};

struct mouse_button: public event {
    mouse::button button;       // cppcheck-suppress unusedStructMember
    components::position point; // cppcheck-suppress unusedStructMember
};

struct mouse_button_down: public mouse_button {};

struct mouse_button_up: public mouse_button {};

} // namespace events

} // namespace sneze
