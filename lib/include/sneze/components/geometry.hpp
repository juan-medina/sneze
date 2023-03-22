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

/**
 * @brief component to hold the position of an entity
 */
struct position {
    //! @brief the x position in logical coordinates
    float x; // cppcheck-suppress unusedStructMember
    //! @brief the y position in logical coordinates
    float y; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component to hold the size of an entity
 */
struct size {
    //! @brief the width in logical coordinates
    float width; // cppcheck-suppress unusedStructMember
    //! @brief the height in logical coordinates
    float height; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component to hold the rectangular area of an entity
 */
struct rect {
    //! @brief the start position of the entity
    struct position position; // cppcheck-suppress unusedStructMember
    //! @brief the size of the entity
    struct size size; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component that represent a line
 */
struct line {
    //! @brief the start position of the line
    struct position to; // cppcheck-suppress unusedStructMember
    //! @brief the thickness of the line
    float thickness; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component that represent a box
 */
struct box {
    //! @brief the start position of the box
    struct position to; // cppcheck-suppress unusedStructMember
    //! @brief the thickness of the box
    float thickness; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component that represent a solid box
 */
struct solid_box {
    //! @brief the start position of the box
    struct position to; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component that represent a box with a border
 */
struct border_box: public solid_box {
    //! @brief the thickness of the border
    float thickness; // cppcheck-suppress unusedStructMember
    //! @brief the color of the border
    components::color color; // cppcheck-suppress unusedStructMember
};

} // namespace sneze::components
