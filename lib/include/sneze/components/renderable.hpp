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

namespace sneze::components {

struct renderable {
    bool visible{true}; // cppcheck-suppress unusedStructMember
    float depth{0.0f};  // cppcheck-suppress unusedStructMember
};

class color {
public:
    using component = unsigned char;

    enum opacity : component { translucent [[maybe_unused]] = 0, opaque = 255 };

    [[nodiscard]] static inline auto
    rgba(const component &red, const component &green, const component &blue, const component &alpha) -> const color {
        return {red, green, blue, alpha};
    }

    [[nodiscard]] static inline auto rgb(const component &red, const component &green, const component &blue) {
        return rgba(red, green, blue, opacity::opaque);
    }

    component r;
    component g;
    component b;
    component a;

    static const color White;
    static const color Black;
    static const color Gray;
    static const color LightGray;
    static const color DarkGray;
    static const color Yellow;
    static const color Gold;
    static const color Orange;
    static const color Pink;
    static const color Red;
    static const color Maroon;
    static const color Green;
    static const color Lime;
    static const color DarkGreen;
    static const color SkyBlue;
    static const color Blue;
    static const color DarkBlue;
    static const color Purple;
    static const color Violet;
    static const color DarkPurple;
    static const color Beige;
    static const color Brown;
    static const color DarkBrown;
    static const color Magenta;
};

} // namespace sneze::components
