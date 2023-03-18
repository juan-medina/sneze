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

#include "sneze/render/texture.hpp"

#include "sneze/render/render.hpp"

#include <filesystem>

#include <SDL_image.h>
#include <SDL_render.h>

namespace sneze {

texture::~texture() {
    texture::end();
}

void texture::end() noexcept {
    logger::trace("texture end");

    if(texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

auto texture::init(const std::string &file) -> result<> {
    if(auto [texture, err] = load_texture(file).ok(); err) {
        logger::error("load texture fail on file: ", file);
        return error{"Can't init texture.", *err};
    } else { // NOLINT(readability-else-after-return)
        texture_ = *texture;
    }

    return true;
}

auto texture::load_texture(const std::string &file_path) -> result<SDL_Texture *const, error> {
    if(auto *rwops = get_render()->get_sdl_rwops(file_path); rwops != nullptr) {
        if(auto *texture = IMG_LoadTexture_RW(get_render()->sdl_renderer(), rwops, 1); texture != nullptr) {
            int width{0};
            int height{0};
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
            size_.height = static_cast<float>(height);
            size_.width = static_cast<float>(width);
            logger::trace("texture loaded: {}, size: {}x{}", file_path, width, height);
            return texture;
        }
        logger::error("error loading texture: {}", IMG_GetError());
        return error("Error loading texture.");
    }
    logger::error("error loading texture: {}", SDL_GetError());
    return error("Error loading texture.");
}

void texture::draw(components::rect origin, components::rect destination, components::color color) {
    if(texture_ != nullptr) [[likely]] {
        const auto src = SDL_Rect{static_cast<int>(origin.position.x),
                                  static_cast<int>(origin.position.y),
                                  static_cast<int>(origin.size.width),
                                  static_cast<int>(origin.size.height)};
        const auto dst = SDL_Rect{static_cast<int>(destination.position.x),
                                  static_cast<int>(destination.position.y),
                                  static_cast<int>(destination.size.width),
                                  static_cast<int>(destination.size.height)};
        SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture_, color.a);
        SDL_RenderCopy(get_render()->sdl_renderer(), texture_, &src, &dst);
    }
}

void texture::draw(components::rect origin,
                   components::rect destination,
                   const bool &flip_x,
                   const bool &flip_y,
                   float rotation,
                   components::color color) {
    if(texture_ != nullptr) [[likely]] {
        const auto src = SDL_Rect{static_cast<int>(origin.position.x),
                                  static_cast<int>(origin.position.y),
                                  static_cast<int>(origin.size.width),
                                  static_cast<int>(origin.size.height)};
        const auto dst = SDL_Rect{static_cast<int>(destination.position.x),
                                  static_cast<int>(destination.position.y),
                                  static_cast<int>(destination.size.width),
                                  static_cast<int>(destination.size.height)};
        SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture_, color.a);

        auto sdl_flip = static_cast<SDL_RendererFlip>(static_cast<int>(flip_y) * SDL_FLIP_VERTICAL
                                                      | static_cast<int>(flip_x) * SDL_FLIP_HORIZONTAL);

        SDL_RenderCopyEx(get_render()->sdl_renderer(), texture_, &src, &dst, rotation, nullptr, sdl_flip);
    }
}

} // namespace sneze
