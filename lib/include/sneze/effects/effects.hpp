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

#include "../components/renderable.hpp"

//! effects namespace
namespace sneze::effects {

/**
 * @brief Alternate color effect
 *
 * This effect will alternate between two colors, it can be used to create a blinking effect
 * or since you can control the time and delay, you can create a fade in/out effect.
 *
 * This works for any renderable object, including labels, sprites, etc.
 */
struct alternate_color {
    //! the color to alternate from, default is white
    class components::color from = components::color::white; // cppcheck-suppress unusedStructMember
    //! the color to alternate to, default is white
    class components::color to = components::color::white; // cppcheck-suppress unusedStructMember
    //! the time to alternate cycles, in milliseconds, default is 150
    float time = 150.0F; // cppcheck-suppress unusedStructMember
    //! the delay/pause between each cycle, in milliseconds, default is 100
    float delay = 100.F; // cppcheck-suppress unusedStructMember
    //! if the effect is paused, default is false
    bool pause = false; // cppcheck-suppress unusedStructMember
    //! the current time, in milliseconds, default is 0
    float current_time = 0.F; // cppcheck-suppress unusedStructMember
};

} // namespace sneze::effects