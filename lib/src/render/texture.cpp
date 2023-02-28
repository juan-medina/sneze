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

#include "sneze/platform/logger.hpp"
#include "sneze/render/render.hpp"

#include <filesystem>

#include <SDL_render.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace fs = std::filesystem;

sneze::texture::~texture() {
    end();
}

void sneze::texture::end() noexcept {
    logger::debug("texture end");

    if(texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

auto sneze::texture::init(const std::string &file) -> sneze::result<> {
    if(const auto file_path = fs::path{file}; fs::exists(file_path)) {
        if(auto [texture, err] = load_texture(file_path.string()).ok(); err) {
            logger::error("load texture fail on file: ", file);
            return error{"Can't init texture.", *err};
        } else {
            texture_ = *texture;
        }
    } else {
        logger::error("load texture fail, file does not exits file: ", file);
        return error{"Can't init texture."};
    }
    return true;
}

auto sneze::texture::load_texture(const std::string &file_path) const -> sneze::result<SDL_Texture *const, error> {
    void *data = nullptr;
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;

    std::int32_t width{0}, height{0}, bytes_per_pixel{0};
    data = stbi_load(file_path.c_str(), &width, &height, &bytes_per_pixel, 0);
    if(!data) {
        logger::error("error loading texture: {}, on file: {}", stbi_failure_reason(), file_path);
        return error{"Error loading texture file."};
    }

    int pitch = width * bytes_per_pixel;
    pitch = (pitch + 3) & ~3;

    std::uint32_t red_mask{0}, green_mask{0}, blue_mask{0}, alpha_mask{0};
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    red_mask = 0x000000FF;
    green_mask = 0x0000FF00;
    blue_mask = 0x00FF0000;
    alpha_mask = (bytes_per_pixel == 4) ? 0xFF000000 : 0;
#else
    int shift = (bytes_per_pixel == 4) ? 0 : 8;
    red_mask = 0xFF000000 >> shift;
    green_mask = 0x00FF0000 >> shift;
    blue_mask = 0x0000FF00 >> shift;
    alpha_mask = 0x000000FF >> shift;
#endif
    auto depth = bytes_per_pixel * 8;
    surface = SDL_CreateRGBSurfaceFrom(data, width, height, depth, pitch, red_mask, green_mask, blue_mask, alpha_mask);
    if(!surface) {
        logger::error("error loading texture: can't create surface");
    } else {
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
        texture = SDL_CreateTextureFromSurface(render_->sdl_renderer(), surface);

        if(!texture) {
            logger::error("error loading texture: can't create texture");
        } else {
            logger::info("texture loaded: {}, size: {}x{} {}bpp", file_path, width, height, bytes_per_pixel);
        }
    }

    if(surface) SDL_FreeSurface(surface);
    if(data) stbi_image_free(data);

    if(texture) {
        return texture;
    } else {
        return error{"Error creating texture."};
    }
}
