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

#include <string>

#include "renderable.hpp"

namespace sneze::components {

struct position {
    float x; // cppcheck-suppress unusedStructMember
    float y; // cppcheck-suppress unusedStructMember
};

struct size {
    float width;  // cppcheck-suppress unusedStructMember
    float height; // cppcheck-suppress unusedStructMember
};

struct rect {
    struct position position; // cppcheck-suppress unusedStructMember
    struct size size;         // cppcheck-suppress unusedStructMember
};

struct line {
    struct position to; // cppcheck-suppress unusedStructMember
    float thickness;    // cppcheck-suppress unusedStructMember
};

struct box {
    struct position to; // cppcheck-suppress unusedStructMember
    float thickness;    // cppcheck-suppress unusedStructMember
};

struct solid_box {
    struct position to; // cppcheck-suppress unusedStructMember
};

struct border_box: public solid_box {
    float thickness;         // cppcheck-suppress unusedStructMember
    components::color color; // cppcheck-suppress unusedStructMember
};

} // namespace sneze::components
