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

namespace sneze {
class world;

namespace events {

struct event {
    sneze::world *world; // cppcheck-suppress unusedStructMember
};

struct application_want_closing: public event {};

struct toggle_fullscreen: public event {};

namespace keyboard {

enum modifier : uint16_t {
    none = 0,
    left_shift = 1,
    right_shift = 2,
    left_control = 4,
    right_control = 8,
    left_alt = 16,
    right_alt = 32,
    shift = left_shift | right_shift,
    control = left_control | right_control,
    alt = left_alt | right_alt,
};

enum key {
    unknown = -1,
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q,
    r,
    s,
    t,
    u,
    v,
    w,
    x,
    y,
    z,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _0,
    enter,
    escape,
    backspace,
    tab,
    space,
    minus,
    equals,
    left_bracket,
    right_bracket,
    back_slash,
    forward_slash,
    hash,
    semicolon,
    back_quote,
    comma,
    period,
    caps_lock,
    f1,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,
    pause,
    exclamation_mark,
    double_quote,
    dollar,
    ampersand,
    single_quote,
    left_parenthesis,
    right_parenthesis,
    asterisk,
    plus,
};

struct key_event: public event {
    key key;           // cppcheck-suppress unusedStructMember
    modifier modifier; // cppcheck-suppress unusedStructMember
};

struct key_down: key_event {};

struct key_up: key_event {};

} // namespace keyboard

} // namespace events

} // namespace sneze