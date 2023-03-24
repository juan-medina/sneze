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

#include "sneze/systems/sdl_events_system.hpp"

#include "sneze/app/world.hpp"
#include "sneze/events/events.hpp"
#include "sneze/platform/logger.hpp"
#include "sneze/render/render.hpp"

#include <SDL_events.h>

namespace sneze {

void sdl_events_system::init(world * /*world*/) {
    logger::trace("init event system");
}

void sdl_events_system::end(world * /*world*/) {
    logger::trace("end event system");
}

void sdl_events_system::update(world *world) {
    using modifier = keyboard::modifier;
    static const auto valid_modifiers = modifier::shift | modifier::control | modifier::alt | modifier::gui;
    auto event_data = SDL_Event{};
    while(SDL_PollEvent(&event_data) != SDL_FALSE) {
        switch(event_data.type) {
        case SDL_QUIT:
            world->emmit<events::application_want_closing>();
            break;
        case SDL_KEYDOWN:
            world->emmit<events::key_down>(event_data.key.keysym.sym,
                                           static_cast<keyboard::mod>(event_data.key.keysym.mod & valid_modifiers));
            break;
        case SDL_KEYUP:
            world->emmit<events::key_up>(event_data.key.keysym.sym,
                                         static_cast<keyboard::mod>(event_data.key.keysym.mod & valid_modifiers));
            break;
        case SDL_MOUSEMOTION:
            world->emmit<events::mouse_moved>(static_cast<float>(event_data.motion.x),
                                              static_cast<float>(event_data.motion.y));
            break;
        case SDL_MOUSEBUTTONUP:
            world->emmit<events::mouse_button_up>(sdl_mouse_button_to_sneze(event_data.button.button),
                                                  static_cast<float>(event_data.button.x),
                                                  static_cast<float>(event_data.button.y));
            break;
        case SDL_MOUSEBUTTONDOWN:
            world->emmit<events::mouse_button_down>(sdl_mouse_button_to_sneze(event_data.button.button),
                                                    static_cast<float>(event_data.button.x),
                                                    static_cast<float>(event_data.button.y));
            break;
        case SDL_WINDOWEVENT:
            switch(event_data.window.event) {
            case SDL_WINDOWEVENT_DISPLAY_CHANGED: {
                auto window = render_->get_window_size();
                auto logical = render_->window_to_logical(window);
                world->emmit<events::window_resized>(window, logical);
            } break;
            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                auto window = components::size{static_cast<float>(event_data.window.data1),
                                               static_cast<float>(event_data.window.data2)};
                auto logical = render_->window_to_logical(window);
                world->emmit<events::window_resized>(window, logical);
            } break;
            }
            break;
        }
    }
}

auto sdl_events_system::sdl_mouse_button_to_sneze(uint8_t button) -> mouse::button {
    auto use_button = mouse::button::unknown;
    switch(button) {
    case SDL_BUTTON_LEFT:
        use_button = mouse::button::left;
        break;
    case SDL_BUTTON_MIDDLE:
        use_button = mouse::button::middle;
        break;
    case SDL_BUTTON_RIGHT:
        use_button = mouse::button::right;
        break;
    case SDL_BUTTON_X1:
        use_button = mouse::button::x1;
        break;
    case SDL_BUTTON_X2:
        use_button = mouse::button::x2;
        break;
    }
    return use_button;
}

} // namespace sneze
