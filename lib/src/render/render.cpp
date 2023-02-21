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

#include <raylib.h>

namespace sneze {

auto render::init(const std::int64_t &width,
                  const std::int64_t &height,
                  const bool &fullscreen,
                  const std::string &title,
                  const components::color &color) -> result<> {
    logger::debug("Creating window");

    if(fullscreen) {
        SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED);
        InitWindow(0, 0, title.c_str());
    } else {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
        InitWindow(static_cast<int>(width), static_cast<int>(height), title.c_str());
    }

    clear_color(color);
    return true;
}

void render::end() {
    fonts_.clear();
    logger::debug("Closing window");
    CloseWindow();
}

void render::begin_frame() {
    BeginDrawing();

    ClearBackground(clear_color_);
}

void render::end_frame() {
    EndDrawing();
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
        auto font_ptr = std::make_shared<font>(font_path);
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

} // namespace sneze
