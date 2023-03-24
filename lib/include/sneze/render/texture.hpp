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

#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../platform/result.hpp"

#include "resource.hpp"

struct SDL_Texture;

namespace sneze {

class render;

/**
 * @brief Class that represents a texture resource
 * @see sneze::render
 * @see sneze::resources_cache
 */
class texture: resource<> {
public:
    /**
     * @brief Construct a new texture object
     * @param render The render object that will be used to create the texture
     */
    explicit texture(class render *render): resource(render){};

    ~texture() override;

    texture(const texture &) = delete;
    texture(texture &&) = delete;
    auto operator=(const texture &) -> texture & = delete;
    auto operator=(texture &&) -> texture & = delete;

    /**
     * @brief Initialize the texture
     * @param file The path to the file that contains the texture
     * @return true if the texture was initialized successfully, error otherwise
     */
    [[nodiscard]] auto init(const std::string &file) -> result<> override;

    /**
     * @brief end the texture
     */
    void end() noexcept override;

    /**
     * @brief Draw the texture
     * @param origin The origin rectangle
     * @param destination The destination rectangle
     * @param color The color to tint the texture, white = no tint
     */
    void draw(components::rect origin, components::rect destination, components::color color);

    /**
     * @brief Draw the texture
     * @param origin The origin rectangle
     * @param destination The destination rectangle
     * @param flip_x Flip the texture in the x axis
     * @param flip_y Flip the texture in the y axis
     * @param rotation The rotation of the texture
     * @param color The color to tint the texture, white = no tint
     */
    void draw(components::rect origin,
              components::rect destination,
              const bool &flip_x,
              const bool &flip_y,
              float rotation,
              components::color color);

    /**
     * @brief Get the size of the texture
     * @return the size of the texture
     */
    [[nodiscard]] auto size() const noexcept -> components::size {
        return size_;
    }

private:
    //! The size of the texture
    components::size size_{0, 0};
    //! the SDL texture
    SDL_Texture *texture_{nullptr};

    /**
     * @brief Load the texture from a file
     * @param file_path The path to the file that contains the texture
     * @return the SDL texture, error otherwise
     */
    [[nodiscard]] auto load_texture(const std::string &file_path) -> result<SDL_Texture *const, error>;
};

} // namespace sneze
