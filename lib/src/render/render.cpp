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

#include "sneze/render/render.hpp"

#include "sneze/platform/logger.hpp"
#include "sneze/render/font.hpp"

#include <SDL.h>

namespace sneze {

auto render::init(const components::size &size,
                  const bool &fullscreen,
                  const int &monitor,
                  const std::string &title,
                  const components::color &color) -> result<> {
    fullscreen_ = fullscreen;

    logger::debug("init SDL");
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger::error("SDL_Init Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

    auto flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    if(fullscreen_) {
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    logger::debug("creating SDL window");
    window_ = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               static_cast<int>(size.width),
                               static_cast<int>(size.height),
                               flags);
    if(window_ == nullptr) {
        logger::error("SDL_CreateWindow Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

    logger::debug("creating SDL renderer");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if(renderer_ == nullptr) {
        logger::error("SDL_CreateRenderer Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

    auto real_size = render::size();
    logger::info("rendering engine initialized. size: {}x{}, mode: {}",
                 real_size.width,
                 real_size.height,
                 fullscreen_ ? "full screen" : "windowed");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    clear_color(color);

    return true;
}

void render::end() {
    logger::debug("ending SDL renderer");
    fonts_.clear();

    if(renderer_ != nullptr) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    logger::debug("destroying SDL window");
    if(window_ != nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

void render::begin_frame() {
    SDL_SetRenderDrawColor(renderer_, clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    SDL_RenderClear(renderer_);
}

void render::end_frame() {
    SDL_RenderPresent(renderer_);
}

[[nodiscard]] auto render::get_font(const std::string &font_path) -> const std::shared_ptr<font> {
    if(auto [fnt, err] = fonts_.get(font_path).ok(); err) {
        logger::error("trying to get a font not loaded: ({})", font_path);
        return nullptr;
    } else {
        return *fnt;
    }
}

void render::draw_label(const components::label &label,
                        const components::position &position,
                        const components::color &color) {
    if(auto font = get_font(label.font); font != nullptr) [[likely]] {
        font->draw_text(label.text, position, label.alignment, label.size, color);
    } else {
        logger::error("trying to draw a label with a not loaded font: ({})", label.font);
    }
}

auto render::load_font(const std::string &font_path) -> result<> {
    logger::info("loading font: ({})", font_path);

    if(auto err = fonts_.load(font_path).ko(); err) {
        logger::error("fail to load font");
        return error("Fail to load Font", *err);
    }

    return true;
}

auto render::unload_font(const std::string &font_path) -> result<> {
    logger::info("unloading font: ({})", font_path);

    if(auto err = fonts_.unload(font_path).ko(); err) {
        logger::error("fail to unload font");
        return error("Fail to unload Font", *err);
    }

    return true;
}

auto render::size() const -> components::size const {
    if(window_ != nullptr) {
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window_, &width, &height);
        return components::size{static_cast<float>(width), static_cast<float>(height)};
    }
    return components::size{0, 0};
}

void render::toggle_fullscreen() {
    fullscreen_ = !fullscreen_;

    if(fullscreen_) {
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(window_, 0);
    }

    auto real_size = render::size();
    logger::debug("toggle full screen / windowed. size: {}x{}, mode: {}",
                  real_size.width,
                  real_size.height,
                  fullscreen_ ? "full screen" : "windowed");
}

auto render::load_texture(const std::string &texture_path) -> result<> {
    logger::info("loading texture: ({})", texture_path);

    if(auto err = textures_.load(texture_path).ko(); err) {
        logger::error("fail to load texture");
        return error("Fail to load Texture", *err);
    }

    return true;
}

auto render::unload_texture(const std::string &texture_path) -> result<> {
    logger::info("unloading texture: ({})", texture_path);

    if(auto err = textures_.unload(texture_path).ko(); err) {
        logger::error("fail to unload texture");
        return error("Fail to unload texture", *err);
    }

    return true;
}
auto render::get_texture(const std::string &texture_path) -> const std::shared_ptr<texture> {
    if(auto [txt, err] = textures_.get(texture_path).ok(); err) {
        logger::error("trying to get a texture not loaded: ({})", texture_path);
        return nullptr;
    } else {
        return *txt;
    }
}
auto render::monitor() const -> int {
    return SDL_GetWindowDisplayIndex(window_);
}

} // namespace sneze