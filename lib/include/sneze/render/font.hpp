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

#include <array>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../platform/result.hpp"

struct SDL_Texture;

namespace sneze {

class render;

struct glyph {
    components::position position; // cppcheck-suppress unusedStructMember
    components::size size;         // cppcheck-suppress unusedStructMember
    components::position offset;   // cppcheck-suppress unusedStructMember
    float advance;                 // cppcheck-suppress unusedStructMember
    int page;                      // cppcheck-suppress unusedStructMember

    [[nodiscard]] static inline constexpr auto valid(const glyph &glyph) -> bool {
        return glyph.size.width > 0 && glyph.size.height > 0;
    }
};

class font {
public:
    explicit font(render *renderer): renderer_{renderer} {};

    ~font();

    font(const font &) = delete;
    font(font &&) = delete;
    auto operator=(const font &) -> font & = delete;
    auto operator=(font &&) -> font & = delete;

    [[nodiscard]] auto init(const std::string &file) -> result<>;

    void end() noexcept;

    void draw_text(const std::string &text,
                   const components::position &position,
                   const float size,
                   const components::color &color);

private:
    static constexpr auto max_pages = 16;
    using params = std::unordered_map<std::string, std::string>;
    using glyphs = std::array<glyph, 256>;
    using pages = std::array<SDL_Texture *, max_pages>;
    using kernings = std::array<std::array<int, 256>, 256>;

    std::string face_{};
    std::filesystem::path font_directory_{};
    glyphs glyphs_{};
    kernings kernings_{};
    int line_height_{0};
    components::position spacing_{0, 0};
    pages pages_{nullptr};
    render *renderer_{nullptr};

    [[nodiscard]] auto tokens(const std::string &line) -> std::pair<std::string, params>;

    [[nodiscard]] auto parse_line(const std::string &type, const params &params) -> bool;

    [[nodiscard]] auto parse_info(const params &params) -> bool;

    [[nodiscard]] auto parse_common(const params &params) -> bool;

    [[nodiscard]] auto parse_page(const params &params) -> bool;

    [[nodiscard]] auto parse_chars(const params &params) const -> bool;

    [[nodiscard]] auto parse_char(const params &params) -> bool;

    [[nodiscard]] auto parse_kernings(const params &params) -> bool;

    [[nodiscard]] auto parse_kerning(const params &params) -> bool;

    [[nodiscard]] inline auto get_value(const params &params, const std::string &key) const -> const std::string;

    [[nodiscard]] inline auto get_int(const params &params, const std::string &key) const -> const int;

    [[nodiscard]] inline auto get_pair(const params &params, const std::string &key) const -> const std::pair<int, int>;

    [[nodiscard]] auto validate_parsing() -> bool;
};

} // namespace sneze
