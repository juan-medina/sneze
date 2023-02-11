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

namespace raylib {
    typedef struct Color Color;
}

namespace sneze {
    class color  {
    public:
        color( const color& ) = default;
        color( color&& ) = default;

        color& operator=( const color& ) = default;
        color& operator=( color&& ) = default;

        static color
        rgba( const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a ) {
            return { r, g, b, a };
        }

        static color rgb( const unsigned char& r, const unsigned char& g, const unsigned char& b ) {
            return rgba( r, g, b, 255 );
        }

        static color White;
        static color Black;
        static color Gray;

        [[nodiscard]] raylib::Color to_raylib() const;

    private:
        color( const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a ):
            r_( r ), g_( g ), b_( b ), a_( a ) {}

    protected:
        unsigned char r_;
        unsigned char g_;
        unsigned char b_;
        unsigned char a_;
    };

} // namespace sneze