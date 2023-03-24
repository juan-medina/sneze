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

#include "sneze/device/keyboard.hpp"

#include <SDL_keyboard.h>

namespace sneze::keyboard {

[[maybe_unused]] const code key::unknown = SDLK_UNKNOWN;
[[maybe_unused]] const code key::a = SDLK_a;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::b = SDLK_b;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::c = SDLK_c;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::d = SDLK_d;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::e = SDLK_e;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f = SDLK_f;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::g = SDLK_g;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::h = SDLK_h;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::i = SDLK_i;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::j = SDLK_j;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::k = SDLK_k;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::l = SDLK_l;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::m = SDLK_m;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::n = SDLK_n;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::o = SDLK_o;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::p = SDLK_p;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::q = SDLK_q;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::r = SDLK_r;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::s = SDLK_s;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::t = SDLK_t;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::u = SDLK_u;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::v = SDLK_v;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::w = SDLK_w;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::x = SDLK_x;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::y = SDLK_y;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::z = SDLK_z;  // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_1 = SDLK_1; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_2 = SDLK_2; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_3 = SDLK_3; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_4 = SDLK_4; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_5 = SDLK_5; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_6 = SDLK_6; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_7 = SDLK_7; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_8 = SDLK_8; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_9 = SDLK_9; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_0 = SDLK_0; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::_return = SDLK_RETURN;
[[maybe_unused]] const code key::escape = SDLK_ESCAPE;
[[maybe_unused]] const code key::backspace = SDLK_BACKSPACE;
[[maybe_unused]] const code key::tab = SDLK_TAB;
[[maybe_unused]] const code key::space = SDLK_SPACE;
[[maybe_unused]] const code key::minus = SDLK_MINUS;
[[maybe_unused]] const code key::equals = SDLK_EQUALS;
[[maybe_unused]] const code key::left_bracket = SDLK_LEFTBRACKET;
[[maybe_unused]] const code key::right_bracket = SDLK_RIGHTBRACKET;
[[maybe_unused]] const code key::back_slash = SDLK_BACKSLASH;
[[maybe_unused]] const code key::forward_slash = SDLK_SLASH;
[[maybe_unused]] const code key::semicolon = SDLK_SEMICOLON;
[[maybe_unused]] const code key::single_quote = SDLK_QUOTE;
[[maybe_unused]] const code key::back_quote = SDLK_BACKQUOTE;
[[maybe_unused]] const code key::comma = SDLK_COMMA;
[[maybe_unused]] const code key::period = SDLK_PERIOD;
[[maybe_unused]] const code key::caps_lock = SDLK_CAPSLOCK;
[[maybe_unused]] const code key::f1 = SDLK_F1; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f2 = SDLK_F2; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f3 = SDLK_F3; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f4 = SDLK_F4; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f5 = SDLK_F5; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f6 = SDLK_F6; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f7 = SDLK_F7; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f8 = SDLK_F8; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f9 = SDLK_F9; // NOLINT(readability-identifier-length)
[[maybe_unused]] const code key::f10 = SDLK_F10;
[[maybe_unused]] const code key::f11 = SDLK_F11;
[[maybe_unused]] const code key::f12 = SDLK_F12;
[[maybe_unused]] const code key::print_screen = SDLK_PRINTSCREEN;
[[maybe_unused]] const code key::scroll_lock = SDLK_SCROLLLOCK;
[[maybe_unused]] const code key::pause = SDLK_PAUSE;
[[maybe_unused]] const code key::exclamation_mark = SDLK_EXCLAIM;
[[maybe_unused]] const code key::double_quote = SDLK_QUOTEDBL;
[[maybe_unused]] const code key::dollar = SDLK_DOLLAR;
[[maybe_unused]] const code key::ampersand = SDLK_AMPERSAND;
[[maybe_unused]] const code key::left_parenthesis = SDLK_LEFTPAREN;
[[maybe_unused]] const code key::right_parenthesis = SDLK_RIGHTPAREN;
[[maybe_unused]] const code key::asterisk = SDLK_ASTERISK;
[[maybe_unused]] const code key::plus = SDLK_PLUS;
[[maybe_unused]] const code key::insert = SDLK_INSERT;
[[maybe_unused]] const code key::home = SDLK_HOME;
[[maybe_unused]] const code key::page_up = SDLK_PAGEUP;
[[maybe_unused]] const code key::supr = SDLK_DELETE;
[[maybe_unused]] const code key::end = SDLK_END;
[[maybe_unused]] const code key::page_down = SDLK_PAGEDOWN;
[[maybe_unused]] const code key::right = SDLK_RIGHT;
[[maybe_unused]] const code key::left = SDLK_LEFT;

auto key::string(const code &key) -> std::string {
    return std::string{SDL_GetKeyName(key)};
}

[[maybe_unused]] const mod modifier::none = KMOD_NONE;
[[maybe_unused]] const mod modifier::left_shift = KMOD_LSHIFT;
[[maybe_unused]] const mod modifier::right_shift = KMOD_RSHIFT;
[[maybe_unused]] const mod modifier::left_control = KMOD_LCTRL;
[[maybe_unused]] const mod modifier::right_control = KMOD_RCTRL;
[[maybe_unused]] const mod modifier::left_alt = KMOD_LALT;
[[maybe_unused]] const mod modifier::right_alt = KMOD_RALT;
[[maybe_unused]] const mod modifier::left_gui = KMOD_LGUI;
[[maybe_unused]] const mod modifier::right_gui = KMOD_RGUI;
[[maybe_unused]] const mod modifier::shift = KMOD_SHIFT;
[[maybe_unused]] const mod modifier::control = KMOD_CTRL;
[[maybe_unused]] const mod modifier::alt = KMOD_ALT;
[[maybe_unused]] const mod modifier::gui = KMOD_GUI;

auto modifier::string(const mod &modifier) -> std::string {
    auto text = std::string{};

    switch(modifier) {
    case modifier::none:
        text = "";
        break;
    case modifier::left_shift:
        text = "Left Shift";
        break;
    case modifier::right_shift:
        text = "Right Shift";
        break;
    case modifier::left_control:
        text = "Left Control";
        break;
    case modifier::right_control:
        text = "Right Control";
        break;
    case modifier::left_alt:
        text = "Left Alt";
        break;
    case modifier::right_alt:
        text = "Right Alt";
        break;
    case modifier::left_gui:
#if defined(__APPLE__)
        text = "Left Command";
#elif defined(__linux__)
        text = "Left Super";
#elif defined(_WIN32)
        text = "Left Windows";
#else
        text = "Left GUI";
#endif
        break;
    case modifier::right_gui:
#if defined(__APPLE__)
        text = "Right Command";
#elif defined(__linux__)
        text = "Right Super";
#elif defined(_WIN32)
        text = "Right Windows";
#else
        text = "Right GUI";
#endif
        break;
    case modifier::shift:
        text = "Shift";
        break;
    case modifier::control:
        text = "Control";
        break;
    case modifier::alt:
        text = "Alt";
        break;
    case modifier::gui:
#if defined(__APPLE__)
        text = "Command";
#elif defined(__linux__)
        text = "Super";
#elif defined(_WIN32)
        text = "Windows";
#else
        text = "GUI";
#endif
        break;
    }

    return text;
}

} // namespace sneze::keyboard
