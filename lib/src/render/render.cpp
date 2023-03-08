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

auto render::init(const components::size &window,
                  const components::size &logical,
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

#if not defined(__linux__)
    if(fullscreen_) {
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
#endif

    logger::debug("creating SDL window");

    window_ = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               static_cast<int>(window.width),
                               static_cast<int>(window.height),
                               flags);
    if(window_ == nullptr) {
        logger::error("SDL_CreateWindow Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

#if defined(__linux__)
    if(fullscreen_) {
        SDL_MaximizeWindow(window_);
    }
#endif

    logger::debug("creating SDL renderer");
    renderer_ = SDL_CreateRenderer(window_, preferred_driver(), SDL_RENDERER_ACCELERATED);
    if(renderer_ == nullptr) {
        logger::error("SDL_CreateRenderer Error: {}", SDL_GetError());
        return error("error initializing rendering engine.");
    }

    SDL_RenderSetLogicalSize(renderer_, static_cast<int>(logical.width), static_cast<int>(logical.height));
    SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "overscan");

    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");

    auto real_size = render::window();
    logger::info("rendering engine initialized. window: {}x{}, mode: {}",
                 real_size.width,
                 real_size.height,
                 fullscreen_ ? "full screen" : "windowed");

#if defined(_WINDOWS)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
#else
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
#endif

    clear_color(color);

    // forcing SDL to recalculate the DPI and viewport
    SDL_Event event;
    memset(&event, 0, sizeof(event));
    event.type = SDL_WINDOWEVENT;
    event.window.event = SDL_WINDOWEVENT_DISPLAY_CHANGED;
    event.window.windowID = SDL_GetWindowID(window_);

    SDL_PushEvent(&event);

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
        return *fnt; // NOLINT(bugprone-unchecked-optional-access)
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

auto render::window() -> components::size const {
    if(window_ != nullptr) {
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window_, &width, &height);
        return components::size{static_cast<float>(width), static_cast<float>(height)};
    }
    return components::size{0, 0};
}

auto render::logical() -> const components::rect {
    return window_to_logical(render::window());
}

auto render::window_to_logical(const components::position &position) -> const components::position {
    float x = 0, y = 0;
    SDL_RenderWindowToLogical(renderer_, static_cast<int>(position.x), static_cast<int>(position.y), &x, &y);

    return components::position{x, y};
}

auto render::window_to_logical(const components::size &size) -> const components::rect {
    auto top_left = window_to_logical(components::position{0, 0});
    auto bottom_right = window_to_logical(components::position{size.width, size.height});

    return components::rect{top_left, components::size{bottom_right.x - top_left.x, bottom_right.y - top_left.y}};
}

void render::toggle_fullscreen() {
    fullscreen_ = !fullscreen_;

    if(fullscreen_) {
#if not defined(__linux__)
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
#else
        SDL_MaximizeWindow(window_);
#endif
    } else {
#if not defined(__linux__)
        SDL_SetWindowFullscreen(window_, 0);
#else
        SDL_RestoreWindow(window_);
#endif
    }

    auto real_size = render::window();
    auto real_logical = render::logical();
    logger::debug("toggle full screen / windowed. size: {}x{}, mode: {}, logical: {}x{}",
                  real_size.width,
                  real_size.height,
                  fullscreen_ ? "full screen" : "windowed",
                  real_logical.size.width,
                  real_logical.size.height);
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
        return *txt; // NOLINT(bugprone-unchecked-optional-access)
    }
}

auto render::monitor() const -> int {
    return SDL_GetWindowDisplayIndex(window_);
}

auto render::preferred_driver() -> int {
    using namespace std::string_literals;
    const auto preferred_drivers = {
        "direct3d12"s,
        "direct3d11"s,
        "direct3d"s,
        "metal"s,
        "opengl"s,
        "opengles2"s,
        "opengles"s,
    };

    auto driver_info = SDL_RendererInfo{};

    for(const auto &driver_name: preferred_drivers) {
        for(int driver_id = 0; driver_id < SDL_GetNumRenderDrivers(); ++driver_id) {
            if(SDL_GetRenderDriverInfo(driver_id, &driver_info) == 0) {
                if(driver_info.name == driver_name) {
                    logger::debug("choosing preferred SDL driver: {}", driver_info.name);
                    return driver_id;
                }
            }
        }
    }

    logger::warning("no preferred SDL driver found, using default");
    return -1;
}
void render::draw_line(const components::line &line,
                       const components::position &position,
                       const components::color &color) {
    if(line.thickness == 1) {
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(
            renderer_, position.x, position.y, position.x + line.size.width, position.y + line.size.height);
        SDL_SetRenderDrawColor(renderer_, clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    } else {
        const float length = std::sqrt(line.size.width * line.size.width + line.size.height * line.size.height);
        const float scale = line.thickness / (2.f * length);
        auto radius = components::position{-line.size.height * scale, line.size.width * scale};

        const auto end_position = components::position{position.x + line.size.width, position.y + line.size.height};

        const auto points = std::vector<components::position>{{
            {position.x - radius.x, position.y - radius.y},
            {position.x + radius.x, position.y + radius.y},
            {end_position.x - radius.x, end_position.y - radius.y},
            {end_position.x + radius.x, end_position.y + radius.y},
        }};

        fill_points_with_triangles(points, color);
    }
}

void render::fill_points_with_triangles(const std::vector<components::position> &points,
                                        const components::color &color) {
    const auto sdl_color = SDL_Color{color.r, color.g, color.b, color.a};

    std::vector<SDL_Vertex> vertexes;
    const auto num_points = static_cast<int>(points.size());
    for(auto i = 2; i < num_points; ++i) {
        if((i % 2) == 0) {
            vertexes.push_back({{points[i].x, points[i].y}, sdl_color, {0, 0}});
            vertexes.push_back({{points[i - 2].x, points[i - 2].y}, sdl_color, {0, 0}});
            vertexes.push_back({{points[i - 1].x, points[i - 1].y}, sdl_color, {0, 0}});
        } else {
            vertexes.push_back({{points[i].x, points[i].y}, sdl_color, {0, 0}});
            vertexes.push_back({{points[i - 1].x, points[i - 1].y}, sdl_color, {0, 0}});
            vertexes.push_back({{points[i - 2].x, points[i - 2].y}, sdl_color, {0, 0}});
        }
    }

    SDL_RenderGeometry(renderer_, nullptr, vertexes.data(), static_cast<int>(vertexes.size()), nullptr, 0);
}

void render::draw_box(const components::box &box,
                      const components::position &position,
                      const components::color &color) {
    auto half_thickness = box.thickness / 2.f;
    draw_line({{box.size.width + box.thickness, 0}, box.thickness}, {position.x - half_thickness, position.y}, color);
    draw_line({{box.size.width + box.thickness, 0}, box.thickness},
              {position.x - half_thickness, position.y + box.size.height},
              color);
    draw_line({{0, box.size.height}, box.thickness}, {position.x, position.y}, color);
    draw_line({{0, box.size.height}, box.thickness}, {position.x + box.size.width, position.y}, color);
}

} // namespace sneze