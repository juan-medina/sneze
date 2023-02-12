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

#include <raylib.h>

namespace sneze {

    class color : public Color {
    public:
        typedef unsigned char component;

        [[maybe_unused]] static const component translucent = 0;
        [[maybe_unused]] static const component opaque = 255;

        color( const color& ) = default;

        color( color&& ) = default;

        explicit color( const Color& color ): Color() {
            Color::r = color.r;
            Color::g = color.g;
            Color::b = color.b;
            Color::a = color.a;
        }

        color& operator=( const color& ) = default;

        color& operator=( color&& ) = default;

        [[nodiscard]] static color
        rgba( const component& red, const component& green, const component& blue, const component& alpha ) {
            return { red, green, blue, alpha };
        }

        [[nodiscard]] static color rgb( const component& red, const component& green, const component& blue ) {
            return rgba( red, green, blue, opaque );
        }

        static color White;
        static color Black;
        static color Gray;

    private:
        color( const component& red, const component& green, const component& blue, const component& alpha ): Color() {
            Color::r = red;
            Color::g = green;
            Color::b = blue;
            Color::a = alpha;
        }
    };

} // namespace sneze