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

#include <SDL.h>

namespace sneze {
void sneze::sdl_events_system::init(sneze::world *) {
    logger::debug("init event system");
}

void sneze::sdl_events_system::end(sneze::world *) {
    logger::debug("end event system");
}

void sneze::sdl_events_system::update(sneze::world *world) {
    SDL_Event eventData;
    namespace keyboard = events::keyboard;

    while(SDL_PollEvent(&eventData)) {
        switch(eventData.type) {
        case SDL_QUIT:
            world->emmit<events::application_want_closing>();
            break;
        case SDL_KEYDOWN:
            world->emmit<keyboard::key_down>(sdl_key_to_key(eventData.key.keysym.sym),
                                             sdl_mod_to_mod(eventData.key.keysym.mod));
            break;
        case SDL_KEYUP:
            world->emmit<keyboard::key_up>(sdl_key_to_key(eventData.key.keysym.sym),
                                           sdl_mod_to_mod(eventData.key.keysym.mod));
            break;
        }
    }
}

auto sdl_events_system::sdl_key_to_key(const int32_t &key) const -> const events::keyboard::key {
    auto result = events::keyboard::key::unknown;
    switch(key) {
    case SDLK_BACKSPACE:
        result = events::keyboard::key::backspace;
        break;
    case SDLK_TAB:
        result = events::keyboard::key::tab;
        break;
    case SDLK_RETURN:
        result = events::keyboard::key::enter;
        break;
    case SDLK_PAUSE:
        result = events::keyboard::key::pause;
        break;
    case SDLK_ESCAPE:
        result = events::keyboard::key::escape;
        break;
    case SDLK_SPACE:
        result = events::keyboard::key::space;
        break;
    case SDLK_EXCLAIM:
        result = events::keyboard::key::exclamation_mark;
        break;
    case SDLK_QUOTEDBL:
        result = events::keyboard::key::double_quote;
        break;
    case SDLK_HASH:
        result = events::keyboard::key::hash;
        break;
    case SDLK_DOLLAR:
        result = events::keyboard::key::dollar;
        break;
    case SDLK_AMPERSAND:
        result = events::keyboard::key::ampersand;
        break;
    case SDLK_QUOTE:
        result = events::keyboard::key::single_quote;
        break;
    case SDLK_LEFTPAREN:
        result = events::keyboard::key::left_parenthesis;
        break;
    case SDLK_RIGHTPAREN:
        result = events::keyboard::key::right_parenthesis;
        break;
    case SDLK_ASTERISK:
        result = events::keyboard::key::asterisk;
        break;
    case SDLK_PLUS:
        result = events::keyboard::key::plus;
        break;
    case SDLK_COMMA:
        result = events::keyboard::key::comma;
        break;
    case SDLK_MINUS:
        result = events::keyboard::key::minus;
        break;
    case SDLK_PERIOD:
        result = events::keyboard::key::period;
        break;
    case SDLK_SLASH:
        result = events::keyboard::key::forward_slash;
        break;
    case SDLK_0:
        result = events::keyboard::key::_0;
        break;
    case SDLK_1:
        result = events::keyboard::key::_1;
        break;
    case SDLK_2:
        result = events::keyboard::key::_2;
        break;
    case SDLK_3:
        result = events::keyboard::key::_3;
        break;
    case SDLK_4:
        result = events::keyboard::key::_4;
        break;
    case SDLK_5:
        result = events::keyboard::key::_5;
        break;
    case SDLK_6:
        result = events::keyboard::key::_6;
        break;
    case SDLK_7:
        result = events::keyboard::key::_7;
        break;
    case SDLK_8:
        result = events::keyboard::key::_8;
        break;
    case SDLK_9:
        result = events::keyboard::key::_9;
        break;
    case SDLK_F1:
        result = events::keyboard::key::f1;
        break;
    case SDLK_F2:
        result = events::keyboard::key::f2;
        break;
    case SDLK_F3:
        result = events::keyboard::key::f3;
        break;
    case SDLK_F4:
        result = events::keyboard::key::f4;
        break;
    case SDLK_F5:
        result = events::keyboard::key::f5;
        break;
    case SDLK_F6:
        result = events::keyboard::key::f6;
        break;
    case SDLK_F7:
        result = events::keyboard::key::f7;
        break;
    case SDLK_F8:
        result = events::keyboard::key::f8;
        break;
    case SDLK_F9:
        result = events::keyboard::key::f9;
        break;
    case SDLK_F10:
        result = events::keyboard::key::f10;
        break;
    case SDLK_F11:
        result = events::keyboard::key::f11;
        break;
    case SDLK_F12:
        result = events::keyboard::key::f12;
        break;
    case SDLK_EQUALS:
        result = events::keyboard::key::equals;
        break;
    case SDLK_LEFTBRACKET:
        result = events::keyboard::key::left_bracket;
        break;
    case SDLK_RIGHTBRACKET:
        result = events::keyboard::key::right_bracket;
        break;
    case SDLK_SEMICOLON:
        result = events::keyboard::key::semicolon;
        break;
    case SDLK_BACKQUOTE:
        result = events::keyboard::key::back_quote;
        break;
    case SDLK_BACKSLASH:
        result = events::keyboard::key::back_slash;
        break;
    case SDLK_CAPSLOCK:
        result = events::keyboard::key::caps_lock;
        break;
    }
    return result;
}

auto sdl_events_system::sdl_mod_to_mod(const uint16_t &mod) const -> const events::keyboard::modifier {
    using modifier = events::keyboard::modifier;
    auto result = static_cast<uint16_t>(modifier::none);

    if(mod & KMOD_LSHIFT) {
        result |= static_cast<uint16_t>(modifier::left_shift);
    }

    if(mod & KMOD_RSHIFT) {
        result |= static_cast<uint16_t>(modifier::right_shift);
    }

    if(mod & KMOD_LCTRL) {
        result |= static_cast<uint16_t>(modifier::left_control);
    }

    if(mod & KMOD_RCTRL) {
        result |= static_cast<uint16_t>(modifier::right_control);
    }

    if(mod & KMOD_LALT) {
        result |= static_cast<uint16_t>(modifier::left_alt);
    }

    if(mod & KMOD_RALT) {
        result |= static_cast<uint16_t>(modifier::right_alt);
    }

    return static_cast<events::keyboard::modifier>(result);
}

} // namespace sneze
