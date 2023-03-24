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

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "../components/geometry.hpp"

#include "resource.hpp"

namespace sneze {

class render;

/**
 * @brief sprite_sheet frame
 * @see sprite_sheet
 */
struct frame {
    //! frame rect
    components::rect rect; // cppcheck-suppress unusedStructMember
    //! frame pivot
    components::position pivot; // cppcheck-suppress unusedStructMember
};

/**
 * @brief sprite_sheet class
 * this class is used to load and draw sprites
 * @see resource
 * @see resources_cache
 */
class sprite_sheet: public resource<bool> {
public:
    /**
     * @brief sprite_sheet constructor
     * @param render the render to use
     */
    explicit sprite_sheet(class render *render): resource(render){};

    ~sprite_sheet() override;

    sprite_sheet(const sprite_sheet &) = delete;
    sprite_sheet(sprite_sheet &&) = delete;

    auto operator=(const sprite_sheet &) -> sprite_sheet & = delete;
    auto operator=(sprite_sheet &&) -> sprite_sheet & = delete;

    /**
     * @brief load a sprite_sheet from a json file or a texture
     * @param uri the uri to the json file or the texture
     * @param is_single_texture if the sprite_sheet is a single texture
     * @return true if the sprite_sheet was loaded correctly, error otherwise
     */
    [[nodiscard]] auto init(const std::string &uri, bool is_single_texture) -> result<> override;

    /**
     * @brief end the sprite_sheet
     */
    void end() override;

    /**
     * @brief draw a sprite
     * @param name the frame name
     * @param position position to draw the sprite
     * @param flip_x if the sprite should be flipped in the x axis
     * @param flip_y if the sprite should be flipped in the y axis
     * @param rotation rotation of the sprite
     * @param scale scale of the sprite
     * @param color tint of the sprite, color::white = no tint
     */
    void draw_sprite(const std::string &name,
                     const components::position &position,
                     const bool &flip_x,
                     const bool &flip_y,
                     const float &rotation,
                     const float &scale,
                     const components::color &color) const;

private:
    //! the frames of the sprite_sheet
    std::unordered_map<std::string, frame> frames_ = {};
    //! the texture of the sprite_sheet
    std::string texture_;
    //! the sprite_sheet directory
    std::filesystem::path sprite_sheet_directory_;

    /**
     * @brief init the sprite_sheet from a json file
     * @param file_path the path to the json file
     * @return true if the sprite_sheet was loaded correctly, error otherwise
     */
    [[nodiscard]] auto init_from_json(const std::filesystem::path &file_path) -> result<>;

    /**
     * @brief init the sprite_sheet from a texture
     * @param file_path the path to the texture
     * @return true if the sprite_sheet was loaded correctly, error otherwise
     */
    [[nodiscard]] auto init_from_texture(const std::filesystem::path &file_path) -> result<>;
};

} // namespace sneze
