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

#include "../platform/result.hpp"
#include "../components/geometry.hpp"
#include "../components/renderable.hpp"

#include "resource.hpp"

struct SDL_Texture;

namespace sneze {

class render;

class texture: resource {
public:
    explicit texture(class render *render): resource(render){};

    ~texture() override;

    texture(const texture &) = delete;
    texture(texture &&) = delete;
    auto operator=(const texture &) -> texture & = delete;
    auto operator=(texture &&) -> texture & = delete;

    [[nodiscard]] auto init(const std::string &file) -> result<> override;

    void end() noexcept override;

    [[nodiscard]] auto sdl_texture() const noexcept -> SDL_Texture *const {
        return texture_;
    };

    void draw(components::rect origin, components::rect destination, components::color color);

private:
    SDL_Texture *texture_{nullptr};

    [[nodiscard]] auto load_texture(const std::string &file_path) const -> result<SDL_Texture *const, error>;
};

}; // namespace sneze
