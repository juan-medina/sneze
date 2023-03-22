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

#include "geometry.hpp"

namespace sneze::components {

/**
 * @brief This is an enum to define the vertical alignment
 */
enum class vertical {
    //! No vertical alignment
    none,
    //! Align vertically to the top
    top,
    //! Align vertically to the center
    center,
    //! Align vertically to the bottom
    bottom
};

/**
 * @brief This is an enum to define the horizontal alignment
 */
enum class horizontal {
    //! No horizontal alignment
    none,
    //! Align horizontally to the left
    left,
    //! Align horizontally to the center
    center,
    //! Align horizontally to the
    right
};

/**
 * @brief This is a struct to define the alignment of a element
 */
struct alignment {
    //! The horizontal alignment, default is left
    enum horizontal horizontal = horizontal::left; // cppcheck-suppress unusedStructMember
    //! The vertical alignment, default is top
    enum vertical vertical = vertical::top; // cppcheck-suppress unusedStructMember
};

/**
 * @brief This define a label, with a font, size and alignment
 */
struct label {
    //! The text to display
    std::string text; // cppcheck-suppress unusedStructMember
    //! The font to use
    std::string font; // cppcheck-suppress unusedStructMember
    //! The size of the font
    float size; // cppcheck-suppress unusedStructMember
    //! The alignment of the text
    struct alignment alignment = {horizontal::left, vertical::top}; // cppcheck-suppress unusedStructMember
};

/**
 * @brief This define an screen anchor, with a horizontal and vertical alignment
 */
struct anchor {
    //! The horizontal alignment
    enum horizontal horizontal = horizontal::none; // cppcheck-suppress unusedStructMember
    //! The vertical alignment
    enum vertical vertical = vertical::none; // cppcheck-suppress unusedStructMember
};

/**
 * @brief This define a layout, with a position
 */
struct layout: public components::position {};

} // namespace sneze::components
