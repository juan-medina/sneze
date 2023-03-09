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
#include <string>

namespace sneze::keyboard {

using code = int32_t;

class key {
public:
    [[maybe_unused]] static const code unknown;
    [[maybe_unused]] static const code a;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code b;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code c;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code d;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code e;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code g;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code h;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code i;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code j;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code k;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code l;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code m;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code n;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code o;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code p;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code q;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code r;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code s;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code t;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code u;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code v;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code w;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code x;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code y;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code z;       // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code _1;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _2;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _3;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _4;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _5;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _6;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _7;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _8;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _9;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _0;      // NOLINT(readability-identifier-naming,readability-identifier-length)
    [[maybe_unused]] static const code _return; // NOLINT(readability-identifier-naming)
    [[maybe_unused]] static const code escape;
    [[maybe_unused]] static const code backspace;
    [[maybe_unused]] static const code tab;
    [[maybe_unused]] static const code space;
    [[maybe_unused]] static const code minus;
    [[maybe_unused]] static const code equals;
    [[maybe_unused]] static const code left_bracket;
    [[maybe_unused]] static const code right_bracket;
    [[maybe_unused]] static const code back_slash;
    [[maybe_unused]] static const code forward_slash;
    [[maybe_unused]] static const code semicolon;
    [[maybe_unused]] static const code back_quote;
    [[maybe_unused]] static const code comma;
    [[maybe_unused]] static const code period;
    [[maybe_unused]] static const code caps_lock;
    [[maybe_unused]] static const code f1; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f2; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f3; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f4; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f5; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f6; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f7; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f8; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f9; // NOLINT(readability-identifier-length)
    [[maybe_unused]] static const code f10;
    [[maybe_unused]] static const code f11;
    [[maybe_unused]] static const code f12;
    [[maybe_unused]] static const code pause;
    [[maybe_unused]] static const code exclamation_mark;
    [[maybe_unused]] static const code double_quote;
    [[maybe_unused]] static const code dollar;
    [[maybe_unused]] static const code ampersand;
    [[maybe_unused]] static const code single_quote;
    [[maybe_unused]] static const code left_parenthesis;
    [[maybe_unused]] static const code right_parenthesis;
    [[maybe_unused]] static const code asterisk;
    [[maybe_unused]] static const code plus;
    [[maybe_unused]] static const code print_screen;
    [[maybe_unused]] static const code scroll_lock;
    [[maybe_unused]] static const code insert;
    [[maybe_unused]] static const code home;
    [[maybe_unused]] static const code page_up;
    [[maybe_unused]] static const code supr;
    [[maybe_unused]] static const code end;
    [[maybe_unused]] static const code page_down;
    [[maybe_unused]] static const code right;
    [[maybe_unused]] static const code left;

    static auto string(const code &key) -> std::string;
};

using mod = uint16_t;

class modifier {
public:
    [[maybe_unused]] static const mod none;
    [[maybe_unused]] static const mod left_shift;
    [[maybe_unused]] static const mod right_shift;
    [[maybe_unused]] static const mod left_control;
    [[maybe_unused]] static const mod right_control;
    [[maybe_unused]] static const mod left_alt;
    [[maybe_unused]] static const mod right_alt;
    [[maybe_unused]] static const mod left_gui;
    [[maybe_unused]] static const mod right_gui;
    [[maybe_unused]] static const mod shift;
    [[maybe_unused]] static const mod control;
    [[maybe_unused]] static const mod alt;
    [[maybe_unused]] static const mod gui;

    static auto string(const mod &modifier) -> std::string;
};

struct key_modifier {
    code key = key::unknown;       // NOLINT(misc-non-private-member-variables-in-classes)
    mod modifier = modifier::none; // NOLINT(misc-non-private-member-variables-in-classes)

    constexpr auto operator==(const key_modifier &other) const {
        if(other.modifier == keyboard::modifier::none) {
            return key == other.key && modifier == keyboard::modifier::none;
        }
        return (key == other.key) && ((modifier & other.modifier) != 0);
    }

    constexpr auto operator==(const code &other) const {
        return key == other && modifier == keyboard::modifier::none;
    }

    [[nodiscard]] auto string() const {
        auto key_str = key::string(key);
        if(auto mod_str = modifier ::string(modifier); !mod_str.empty()) {
            return mod_str + "+" + key_str;
        }
        return key_str;
    }
};
} // namespace sneze::keyboard
