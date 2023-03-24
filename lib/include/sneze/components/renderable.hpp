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

/**
 * @brief component that indicates that the entity is renderable
 *
 * our rendering system is based on the renderable::depth of the entity, acting as a z-index
 * renderable::visible is used to hide the entity from the rendering system
 */
struct renderable {
    //! the depth of the entity
    float depth{0.0F};  // cppcheck-suppress unusedStructMember
    //! the visibility of the entity
    bool visible{true}; // cppcheck-suppress unusedStructMember
};

/**
 * @brief component that indicates the color of the entity
 */
class color {
public:
    //! represents the color components
    using component = unsigned char;

    //! represents the opacity of the color
    enum opacity : component { translucent [[maybe_unused]] = 0, opaque = 255 };

    /**
     * @brief create a color from the given components
     *
     * @param red_component the red component of the color
     * @param green_component the green component of the color
     * @param blue_component the blue component of the color
     * @param alpha_component the alpha component of the color
     * @return the color created from the given components
     */
    [[nodiscard]] static inline auto rgba(const component &red_component,
                                          const component &green_component,
                                          const component &blue_component,
                                          const component &alpha_component) -> color {
        return {red_component, green_component, blue_component, alpha_component};
    }

    /**
     * @brief create a color from the given components without alpha, setting it to opaque
     *
     * @param red_component the red component of the color
     * @param green_component the green component of the color
     * @param blue_component the blue component of the color
     * @return the color created from the given components
     */
    [[nodiscard]] static inline auto
    rgb(const component &red_component, const component &green_component, const component &blue_component) {
        return rgba(red_component, green_component, blue_component, opacity::opaque);
    }

    /**
     * @brief create a color base on the existing color and changing the alpha component
     * @param alpha the alpha component of the color, in the range of 0 to 255
     * @return the color with the new alpha component
     */
    [[nodiscard]] inline auto alpha(component alpha) const -> color {
        return rgba(r, g, b, alpha);
    }

    /**
     * @brief create a color base on the existing color and changing the alpha component
     * @param alpha the alpha component of the color, in the range of 0.0F to 1.0F
     * @return
     */
    [[nodiscard]] inline auto alpha(float alpha) const -> color {
        return rgba(r, g, b, static_cast<component>(alpha * static_cast<component>(opacity::opaque)));
    }

    /**
     * @brief create a color base on the existing color and giving an opacity
     * @param alpha the opacity of the color
     * @return
     */
    [[nodiscard]] inline auto alpha(opacity alpha) const -> color {
        return rgba(r, g, b, static_cast<component>(alpha));
    }

    /**
     * @brief create a color base on the existing blend with the given color by the given factor
     * @param other the color to blend with
     * @param factor the factor to blend the color, in the range of 0.0F to 1.0F
     */
    void blend(const color &other, float factor) {
        r = static_cast<component>(static_cast<float>(r) * (1.0F - factor) + static_cast<float>(other.r) * factor);
        g = static_cast<component>(static_cast<float>(g) * (1.0F - factor) + static_cast<float>(other.g) * factor);
        b = static_cast<component>(static_cast<float>(b) * (1.0F - factor) + static_cast<float>(other.b) * factor);
        a = static_cast<component>(static_cast<float>(a) * (1.0F - factor) + static_cast<float>(other.a) * factor);
    }

    //! the red component of the color
    component r; // NOLINT(misc-non-private-member-variables-in-classes)

    //! the green component of the color
    component g; // NOLINT(misc-non-private-member-variables-in-classes)

    //! the blue component of the color
    component b; // NOLINT(misc-non-private-member-variables-in-classes)

    //! the alpha component of the color
    component a; // NOLINT(misc-non-private-member-variables-in-classes)

    //! the color white
    static const color white;

    //! the color black
    static const color black;

    //! the color gray
    static const color gray;

    //! the color light gray
    static const color light_gray;

    //! the color dark gray
    static const color dark_gray;

    //! the color yellow
    static const color yellow;

    //! the color gold
    static const color gold;

    //! the color orange
    static const color orange;

    //! the color pink
    static const color pink;

    //! the color red
    static const color red;

    //! the color maroon
    static const color maroon;

    //! the color green
    static const color green;

    //! the color lime
    static const color lime;

    //! the color dark green
    static const color dark_green;

    //! the color sky blue
    static const color sky_blue;

    //! the color blue
    static const color blue;

    //! the color dark blue
    static const color dark_blue;

    //! the color purple
    static const color purple;

    //! the color violet
    static const color violet;

    //! the color dark purple
    static const color dark_purple;

    //! the color beige
    static const color beige;

    //! the color brown
    static const color brown;

    //! the color dark brown
    static const color dark_brown;

    //! the color magenta
    static const color magenta;

    //! the color untinted, which is white
    static const color untinted;
};

/**
 * @brief component that indicates a sprite
 */
struct sprite {
    //! the file for the sprite, single file name for single sprites or the sprite sheet
    std::string file;              // cppcheck-suppress unusedStructMember

    //! the frame of the sprite, if the sprite is a sprite sheet, if not "default"
    std::string frame = "default"; // cppcheck-suppress unusedStructMember

    //! if the sprite should be flipped on the x axis
    bool flip_x{false};            // cppcheck-suppress unusedStructMember

    //! if the sprite should be flipped on the y axis
    bool flip_y{false};            // cppcheck-suppress unusedStructMember

    //! the scale of the sprite
    float scale{1.0F};             // cppcheck-suppress unusedStructMember

    //! the rotation of the sprite, in degrees
    float rotation{0.0F};          // cppcheck-suppress unusedStructMember
};

} // namespace sneze::components
