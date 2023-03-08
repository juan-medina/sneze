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

#include "../app/world.hpp"
#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../components/ui.hpp"
#include "../platform/result.hpp"

#include "font.hpp"
#include "texture.hpp"

struct SDL_Renderer;
struct SDL_Window;

namespace sneze {

class render {
public:
    render(): fonts_{this}, textures_{this} {};
    ~render() = default;

    render(const render &) = delete;
    render(render &&) = delete;

    auto operator=(const render &) -> render & = delete;
    auto operator=(render &&) -> render & = delete;

    [[maybe_unused]] void clear_color(const components::color &color) {
        clear_color_ = color;
    }

    [[nodiscard]] auto init(const components::size &size,
                            const components::size &logical,
                            const bool &fullscreen,
                            const int &monitor,
                            const std::string &title,
                            const components::color &color) -> result<>;

    void end();

    void begin_frame();

    void end_frame();

    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    [[maybe_unused]] auto unload_font(const std::string &font_path) -> result<>;

    [[maybe_unused]] [[nodiscard]] auto load_texture(const std::string &texture_path) -> result<>;

    [[maybe_unused]] auto unload_texture(const std::string &texture_path) -> result<>;

    void
    draw_label(const components::label &label, const components::position &position, const components::color &color);

    void draw_line(const components::line &line, const components::position &position, const components::color &color);

    void draw_box(const components::box &box, const components::position &position, const components::color &color);

    [[nodiscard]] auto window() -> const components::size;

    [[nodiscard]] auto logical() -> const components::rect;

    [[nodiscard]] auto window_to_logical(const components::position &position) -> const components::position;

    [[nodiscard]] auto window_to_logical(const components::size &size) -> const components::rect;

    [[nodiscard]] auto monitor() const -> int;

    [[nodiscard]] auto fullscreen() const {
        return fullscreen_;
    }

    [[nodiscard]] auto sdl_renderer() -> SDL_Renderer * {
        return renderer_;
    }

    void toggle_fullscreen();
    friend class font;

protected:
    [[nodiscard]] auto get_texture(const std::string &texture_path) -> const std::shared_ptr<texture>;

private:
    resources_cache<font> fonts_;
    resources_cache<texture> textures_;

    [[nodiscard]] auto get_font(const std::string &font_path) -> const std::shared_ptr<font>;

    [[nodiscard]] auto preferred_driver() -> int;

    void fill_points_with_triangles(const std::vector<components::position> &points, const components::color &color);

    components::color clear_color_ = components::color::black;

    bool fullscreen_ = false;
    SDL_Window *window_ = {nullptr};
    SDL_Renderer *renderer_ = {nullptr};
};

} // namespace sneze
