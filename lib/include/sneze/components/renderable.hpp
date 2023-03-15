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

namespace sneze::components {

struct renderable {
    float depth{0.0F};  // cppcheck-suppress unusedStructMember
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

    [[nodiscard]] inline auto alpha(component alpha) const -> color {
        return rgba(r, g, b, alpha);
    }

    [[nodiscard]] inline auto alpha(float alpha) const -> color {
        return rgba(r, g, b, static_cast<component>(alpha * static_cast<component>(opacity::opaque)));
    }

    [[nodiscard]] inline auto alpha(opacity alpha) const -> color {
        return rgba(r, g, b, static_cast<component>(alpha));
    }

    void blend(const color &other, float factor) {
        r = static_cast<component>(static_cast<float>(r) * (1.0F - factor) + static_cast<float>(other.r) * factor);
        g = static_cast<component>(static_cast<float>(g) * (1.0F - factor) + static_cast<float>(other.g) * factor);
        b = static_cast<component>(static_cast<float>(b) * (1.0F - factor) + static_cast<float>(other.b) * factor);
        a = static_cast<component>(static_cast<float>(a) * (1.0F - factor) + static_cast<float>(other.a) * factor);
    }

    component r; // NOLINT(misc-non-private-member-variables-in-classes)
    component g; // NOLINT(misc-non-private-member-variables-in-classes)
    component b; // NOLINT(misc-non-private-member-variables-in-classes)
    component a; // NOLINT(misc-non-private-member-variables-in-classes)

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

struct sprite {
    std::string sprite_sheet; // cppcheck-suppress unusedStructMember
    std::string frame;        // cppcheck-suppress unusedStructMember
    bool flip_x{false};       // cppcheck-suppress unusedStructMember
    bool flip_y{false};       // cppcheck-suppress unusedStructMember
    float scale{1.0F};        // cppcheck-suppress unusedStructMember
    float rotation{0.0F};     // cppcheck-suppress unusedStructMember
};

} // namespace sneze::components
