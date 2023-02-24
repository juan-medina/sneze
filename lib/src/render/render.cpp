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

namespace sneze {

auto render::init(const components::size &size,
                  const bool &fullscreen,
                  const std::string &title,
                  const components::color &color) -> result<> {
    fullscreen_ = fullscreen;

    logger::debug("init SDL");
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger::error("SDL_Init Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

    auto flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if(fullscreen_) {
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    logger::debug("creating SDL window");
    window_ = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
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
    fonts_.clear();

    logger::debug("ending SDL renderer");
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

[[nodiscard]] auto render::get_font(const std::string &font_path) -> const auto {
    if(auto it = fonts_.find(font_path); it == fonts_.end()) {
        logger::error("trying to get a font not loaded: ({})", font_path);
        return std::shared_ptr<font>{nullptr};
    } else {
        return it->second;
    }
}

void render::draw_label(const components::label &label,
                        const components::position &position,
                        const components::color &color) {
    if(auto font = get_font(label.font); font != nullptr && font->valid()) [[likely]] {
        font->draw_text(label.text, position, label.size, color);
    } else {
        logger::error("trying to draw a label with a not loaded font: ({})", label.font);
    }
}

auto render::load_font(const std::string &font_path) -> result<> {
    logger::debug("loading font: ({})", font_path);
    if(auto it = fonts_.find(font_path); it == fonts_.end()) {
        auto font_ptr = std::make_shared<font>(renderer_, font_path);
        if(!font_ptr->valid()) {
            logger::error("failed to load font: {}", font_path);
            font_ptr.reset();
            return error("Font not valid.");
        }
        fonts_.insert({font_path, font_ptr});
    } else {
        logger::warning("font already loaded: {}", font_path);
    }

    return true;
}

void render::unload_font(const std::string &font_path) {
    logger::debug("unloading font: ({})", font_path);
    if(auto it = fonts_.find(font_path); it == fonts_.end()) {
        logger::warning("unloading font not loaded: {}", font_path);
    } else {
        fonts_.erase(it);
    }
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

} // namespace sneze