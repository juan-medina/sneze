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

namespace sneze::keyboard {

using code = int32_t;

class key {
public:
    [[maybe_unused]] static const code unknown;
    [[maybe_unused]] static const code a;
    [[maybe_unused]] static const code b;
    [[maybe_unused]] static const code c;
    [[maybe_unused]] static const code d;
    [[maybe_unused]] static const code e;
    [[maybe_unused]] static const code f;
    [[maybe_unused]] static const code g;
    [[maybe_unused]] static const code h;
    [[maybe_unused]] static const code i;
    [[maybe_unused]] static const code j;
    [[maybe_unused]] static const code k;
    [[maybe_unused]] static const code l;
    [[maybe_unused]] static const code m;
    [[maybe_unused]] static const code n;
    [[maybe_unused]] static const code o;
    [[maybe_unused]] static const code p;
    [[maybe_unused]] static const code q;
    [[maybe_unused]] static const code r;
    [[maybe_unused]] static const code s;
    [[maybe_unused]] static const code t;
    [[maybe_unused]] static const code u;
    [[maybe_unused]] static const code v;
    [[maybe_unused]] static const code w;
    [[maybe_unused]] static const code x;
    [[maybe_unused]] static const code y;
    [[maybe_unused]] static const code z;
    [[maybe_unused]] static const code _1;
    [[maybe_unused]] static const code _2;
    [[maybe_unused]] static const code _3;
    [[maybe_unused]] static const code _4;
    [[maybe_unused]] static const code _5;
    [[maybe_unused]] static const code _6;
    [[maybe_unused]] static const code _7;
    [[maybe_unused]] static const code _8;
    [[maybe_unused]] static const code _9;
    [[maybe_unused]] static const code _0;
    [[maybe_unused]] static const code enter;
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
    [[maybe_unused]] static const code f1;
    [[maybe_unused]] static const code f2;
    [[maybe_unused]] static const code f3;
    [[maybe_unused]] static const code f4;
    [[maybe_unused]] static const code f5;
    [[maybe_unused]] static const code f6;
    [[maybe_unused]] static const code f7;
    [[maybe_unused]] static const code f8;
    [[maybe_unused]] static const code f9;
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
    [[maybe_unused]] static const mod shift;
    [[maybe_unused]] static const mod control;
    [[maybe_unused]] static const mod alt;
};

struct key_mod {
    keyboard::code key_code; // cppcheck-suppress unusedStructMember
    keyboard::mod key_mod;   // cppcheck-suppress unusedStructMember
};

} // namespace sneze::keyboard