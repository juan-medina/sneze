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

#include "sneze/embedded/embedded.hpp"
#include "sneze/platform/logger.hpp"
#include "sneze/platform/span_istream.hpp"
#include "sneze/render/font.hpp"

#include <fstream>

#include <binary_resources/binary.hpp>
#include <SDL.h>
#include <SDL_image.h>

namespace sneze {

auto render::init(const components::size &window,
                  const components::size &logical,
                  const bool &fullscreen,
                  const int &monitor,
                  const std::string &title,
                  const std::string &icon,
                  const components::color &color) -> result<> {
    init_embedded_data();

    fullscreen_ = fullscreen;

    logger::trace("init SDL");
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger::error("SDL_Init Error: {}", SDL_GetError());
        return error("Error initializing rendering engine.");
    }

    auto flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

#if not defined(__linux__)
    if(fullscreen_) {
        flags = static_cast<SDL_WindowFlags>(flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
#endif

    logger::trace("creating SDL window");

    window_ = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               SDL_WINDOWPOS_CENTERED_DISPLAY(monitor),
                               static_cast<int>(window.width),
                               static_cast<int>(window.height),
                               flags);
    if(window_ == nullptr) {
        logger::error("SDL_CreateWindow Error: {}", SDL_GetError());
        return error("Error creating window.");
    }

    if(auto err = handle_icon(icon).ko(); err) {
        logger::error("error handling icon");
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        return error("Error handling icon.", *err);
    }

#if defined(__linux__)
    if(fullscreen_) {
        SDL_MaximizeWindow(window_);
    }
#endif

    logger::trace("creating SDL renderer");
    renderer_ = SDL_CreateRenderer(window_, preferred_driver(), SDL_RENDERER_ACCELERATED);
    if(renderer_ == nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        logger::error("SDL_CreateRenderer Error: {}", SDL_GetError());
        return error("Error creating device render.");
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
    logger::trace("ending SDL renderer");
    fonts_.clear();

    if(renderer_ != nullptr) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    logger::trace("destroying SDL window");
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

[[nodiscard]] auto render::get_font(const std::string &font_path) -> std::shared_ptr<font> {
    if(auto [fnt, err] = fonts_.get(font_path).ok(); !err) {
        return *fnt; // NOLINT(bugprone-unchecked-optional-access)
    }
    logger::error("trying to get a font not loaded: ({})", font_path);
    return nullptr;
}

void render::draw_label(const components::label &label,
                        const components::position &from,
                        const components::color &color) {
    if(auto font = get_font(label.font); font != nullptr) [[likely]] {
        font->draw_text(label.text, from, label.alignment, label.size, color);
    } else {
        logger::error("trying to draw a label with a not loaded font: ({})", label.font);
    }
}

auto render::load_font(const std::string &font_path) -> result<> {
    logger::debug("loading font: ({})", font_path);

    if(auto err = fonts_.load(font_path).ko(); err) {
        logger::error("fail to load font");
        return error("Fail to load Font", *err);
    }

    return true;
}

auto render::unload_font(const std::string &font_path) -> result<> {
    logger::debug("unloading font: ({})", font_path);

    if(auto err = fonts_.unload(font_path).ko(); err) {
        logger::error("fail to unload font");
        return error("Fail to unload Font", *err);
    }

    return true;
}

auto render::window() -> components::size {
    if(window_ != nullptr) {
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window_, &width, &height);
        return components::size{static_cast<float>(width), static_cast<float>(height)};
    }
    return components::size{0, 0};
}

auto render::logical() -> components::rect {
    return window_to_logical(render::window());
}

auto render::window_to_logical(const components::position &position) -> components::position {
    float at_x = 0;
    float at_y = 0;
    SDL_RenderWindowToLogical(renderer_, static_cast<int>(position.x), static_cast<int>(position.y), &at_x, &at_y);
    return components::position{at_x, at_y};
}

auto render::window_to_logical(const components::size &size) -> components::rect {
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
    logger::trace("toggle full screen / windowed. size: {}x{}, mode: {}, logical: {}x{}",
                  real_size.width,
                  real_size.height,
                  fullscreen_ ? "full screen" : "windowed",
                  real_logical.size.width,
                  real_logical.size.height);
}

auto render::load_texture(const std::string &texture_path) -> result<> {
    logger::debug("loading texture: ({})", texture_path);

    if(auto err = textures_.load(texture_path).ko(); err) {
        logger::error("fail to load texture");
        return error("Fail to load Texture", *err);
    }

    return true;
}

auto render::unload_texture(const std::string &texture_path) -> result<> {
    logger::debug("unloading texture: ({})", texture_path);

    if(auto err = textures_.unload(texture_path).ko(); err) {
        logger::error("fail to unload texture");
        return error("Fail to unload texture", *err);
    }

    return true;
}

auto render::load_sprite_sheet(const std::string &sprite_sheet_path) -> result<> {
    logger::debug("loading sprite sheet: ({})", sprite_sheet_path);

    if(auto err = sprite_sheets_.load(sprite_sheet_path, false).ko(); err) {
        logger::error("fail to load sprite sheet");
        return error("Fail to load Sprite Sheet", *err);
    }

    return true;
}

auto render::load_sprite(const std::string &sprite_path) -> result<> {
    logger::debug("loading sprite: ({})", sprite_path);

    if(auto err = sprite_sheets_.load(sprite_path, true).ko(); err) {
        logger::error("fail to load sprite");
        return error("Fail to load Sprite", *err);
    }

    return true;
}

auto render::unload_sprite_sheet(const std::string &sprite_sheet_path) -> result<> {
    logger::debug("unloading sprite sheet: ({})", sprite_sheet_path);

    if(auto err = sprite_sheets_.unload(sprite_sheet_path).ko(); err) {
        logger::error("fail to unload sprite sheet");
        return error("Fail to unload Sprite Sheet", *err);
    }

    return true;
}

auto render::unload_sprite(const std::string &sprite_path) -> result<> {
    logger::debug("unloading sprite: ({})", sprite_path);

    if(auto err = sprite_sheets_.unload(sprite_path).ko(); err) {
        logger::error("fail to unload sprite");
        return error("Fail to unload Sprite", *err);
    }

    return true;
}

auto render::get_texture(const std::string &texture_path) -> std::shared_ptr<texture> {
    if(auto [txt, err] = textures_.get(texture_path).ok(); !err) {
        return *txt; // NOLINT(bugprone-unchecked-optional-access)
    }
    logger::error("trying to get a texture not loaded: ({})", texture_path);
    return nullptr;
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

void render::draw_line(const components::line &line, const components::position &from, const components::color &color) {
    const components::size size{line.to.x - from.x, line.to.y - from.y};
    if(line.thickness == 1) {
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(renderer_, from.x, from.y, from.x + size.width, from.y + size.height);
        SDL_SetRenderDrawColor(renderer_, clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    } else {
        const float length = std::sqrt(size.width * size.width + size.height * size.height);
        const float scale = line.thickness / (2.F * length);
        auto radius = components::position{-size.height * scale, size.width * scale};

        const auto points = std::vector<components::position>{{
            {from.x - radius.x, from.y - radius.y},
            {from.x + radius.x, from.y + radius.y},
            {line.to.x - radius.x, line.to.y - radius.y},
            {line.to.x + radius.x, line.to.y + radius.y},
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

void render::draw_box(const components::box &box, const components::position &from, const components::color &color) {
    auto half_thickness = box.thickness / 2.F;
    const components::size size{box.to.x - from.x, box.to.y - from.y};

    draw_line(
        {{from.x + size.width + half_thickness, from.y}, box.thickness}, {from.x - half_thickness, from.y}, color);
    draw_line({{from.x, from.y + size.height}, box.thickness}, {from.x, from.y}, color);
    draw_line({{from.x + size.width + half_thickness, from.y + size.height}, box.thickness},
              {from.x - half_thickness, from.y + size.height},
              color);
    draw_line({{from.x + size.width, from.y + size.height}, box.thickness}, {from.x + size.width, from.y}, color);
}
void render::draw_solid_box(const components::solid_box &box,
                            const components::position &from,
                            const components::color &color) {
    const auto points = std::vector<components::position>{{
        {from.x, from.y},
        {from.x, box.to.y},
        {box.to.x, box.to.y},
        {box.to.x, from.y},
        {from.x, from.y},
    }};

    fill_points_with_triangles(points, color);
}
void render::draw_border_box(const components::border_box &box,
                             const components::position &from,
                             const components::color &color) {
    draw_solid_box({box.to}, from, color);

    draw_box({box.to, box.thickness}, from, box.color);
}

void render::draw_sprite(components::sprite &sprite, const components::position &from, const components::color &color) {
    if(auto sprite_sheet = get_sprite_sheet(sprite.file); sprite_sheet != nullptr) [[likely]] {
        return sprite_sheet->draw_sprite(
            sprite.frame, from, sprite.flip_x, sprite.flip_y, sprite.rotation, sprite.scale, color);
    } else {
        logger::error("trying to draw a sprite with a not loaded sprite sheet: ({})", sprite.file);
    }
}

auto render::get_sprite_sheet(const std::string &sprite_sheet_path) -> std::shared_ptr<sprite_sheet> {
    if(auto [sprite_sheet, err] = sprite_sheets_.get(sprite_sheet_path).ok(); !err) {
        return *sprite_sheet; // NOLINT(bugprone-unchecked-optional-access)
    }
    logger::error("trying to get a font not loaded: ({})", sprite_sheet_path);
    return nullptr;
}

auto render::get_sdl_rwops(const std::string &path) -> SDL_RWops * {
    logger::trace("get sdl rwops for path: ({})", path);
    if(auto data = get_from_embedded_data(path); data) {
        logger::trace("loading from embedded data: ({})", path);
        return get_embedded_sdl_rwops(*data);
    }
    logger::trace("loading from file: ({})", path);
    return SDL_RWFromFile(path.c_str(), "r");
}

[[maybe_unused]] auto render::get_istream(const std::string &path) -> std::unique_ptr<std::istream> {
    logger::trace("get istream for path: ({})", path);
    if(auto data = get_from_embedded_data(path); data) {
        logger::trace("loading from embedded data: ({})", path);
        return get_embedded_istream(*data);
    }
    logger::trace("loading from file: ({})", path);
    return std::make_unique<std::ifstream>(std::ifstream{path});
}

void render::free_sdl_rwops(SDL_RWops *rwops) {
    SDL_RWclose(rwops);
}

auto render::handle_icon(const std::string &icon) -> result<> {
    if(SDL_RWops *rwops = get_sdl_rwops(icon); rwops != nullptr) {
        if(auto *const icon_surface = IMG_Load_RW(rwops, SDL_FALSE); icon_surface != nullptr) {
            SDL_SetWindowIcon(window_, icon_surface);
            SDL_FreeSurface(icon_surface);
            free_sdl_rwops(rwops);
        } else {
            free_sdl_rwops(rwops);
            logger::error("can't load window icon.", IMG_GetError());
            return error("Error can't load window icon.");
        }
    } else {
        logger::error("can't load window icon: {}", SDL_GetError());
        return error("Error can't load window icon.");
    }

    return true;
}

auto render::get_embedded_sdl_rwops(std::span<std::byte const> &data) -> SDL_RWops * {
    // NOLINTNEXTLINE(google-readability-casting)
    return SDL_RWFromMem((void *)data.data(), static_cast<int>(data.size())); // cppcheck-suppress cstyleCast
}

auto render::get_embedded_istream(std::span<std::byte const> &data) -> std::unique_ptr<std::istream> {
    return std::make_unique<span_istream>(data);
}

void render::add_to_embedded_data(const std::string &path, const std::span<std::byte const> &data) {
    if(embedded_data_.contains(path)) [[unlikely]] {
        logger::warning("trying to add a file to embedded data that already exists: ({})", path);
        return;
    }

    embedded_data_.emplace(path, data);
}

auto render::get_from_embedded_data(const std::string &path) -> std::optional<std::span<std::byte const>> {
    if(auto data = embedded_data_.find(path); data != embedded_data_.end()) [[likely]] {
        return data->second;
    }
    return std::nullopt;
}

void render::init_embedded_data() {
    logger::info("initializing embedded data");
    add_to_embedded_data(embedded::sneze_logo, embedded::sneze_logo_data());
    add_to_embedded_data(embedded::mono_font, embedded::mono_font_data());
    add_to_embedded_data(embedded::mono_font_texture, embedded::mono_font_texture_data());
}

auto render::file_exists(const std::string &path) -> bool {
    if(auto data = get_from_embedded_data(path); data) {
        return true;
    }

    namespace fs = std::filesystem;
    if(const auto file_path = fs::path{path}; fs::exists(file_path)) {
        return true;
    }

    return false;
}

auto render::get_parent(const std::string &path) -> std::filesystem::path {
    namespace fs = std::filesystem;
    if(auto data = get_from_embedded_data(path); data) {
        return fs::path{""};
    }
    return fs::path{path}.parent_path();
}

} // namespace sneze