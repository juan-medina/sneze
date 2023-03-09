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
    float depth{0.0f};  // cppcheck-suppress unusedStructMember
    bool visible{true}; // cppcheck-suppress unusedStructMember
};

class color {
public:
    using component = unsigned char;

    enum opacity : component { translucent [[maybe_unused]] = 0, opaque = 255 };

    [[nodiscard]] static inline auto rgba(const component &red_component,
                                          const component &green_component,
                                          const component &blue_component,
                                          const component &alpha_component) -> color {
        return {red_component, green_component, blue_component, alpha_component};
    }

    [[nodiscard]] static inline auto
    rgb(const component &red_component, const component &green_component, const component &blue_component) {
        return rgba(red_component, green_component, blue_component, opacity::opaque);
    }

    component r;
    component g;
    component b;
    component a;

    static const color white;
    static const color black;
    static const color gray;
    static const color light_gray;
    static const color dark_gray;
    static const color yellow;
    static const color gold;
    static const color orange;
    static const color pink;
    static const color red;
    static const color maroon;
    static const color green;
    static const color lime;
    static const color dark_green;
    static const color sky_blue;
    static const color blue;
    static const color dark_blue;
    static const color purple;
    static const color violet;
    static const color dark_purple;
    static const color beige;
    static const color brown;
    static const color dark_brown;
    static const color magenta;
};

} // namespace sneze::components
