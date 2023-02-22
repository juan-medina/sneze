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

#include <entt/entt.hpp>

#include "../app/world.hpp"
#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../components/ui.hpp"
#include "../platform/result.hpp"

#include "font.hpp"

namespace sneze {

class render {
public:
    render() = default;
    ~render() = default;

    render(const render &) = delete;
    render(render &&) = delete;

    auto operator=(const render &) -> render & = delete;
    auto operator=(render &&) -> render & = delete;

    [[maybe_unused]] void clear_color(const components::color &color) {
        clear_color_ = color;
    }

    [[nodiscard]] auto init(const components::size &size,
                            const components::position &placement,
                            const int &monitor,
                            const bool &fullscreen,
                            const std::string &title,
                            const components::color &color) -> result<>;

    void end();

    void begin_frame();

    void end_frame();

    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    [[maybe_unused]] void unload_font(const std::string &font_path);

    void
    draw_label(const components::label &label, const components::position &position, const components::color &color);

    [[nodiscard]] auto placement() const -> components::position;

    [[nodiscard]] auto size() const -> components::size const;

    [[nodiscard]] auto monitor() const -> int const;


    [[nodiscard]] auto fullscreen() const -> bool const {
        return fullscreen_;
    }

private:
    [[nodiscard]] auto get_font(const std::string &font_path) -> const auto;

    components::color clear_color_ = components::color::Black;

    std::unordered_map<std::string, std::shared_ptr<font>> fonts_;

    bool fullscreen_ = false;
};

} // namespace sneze
