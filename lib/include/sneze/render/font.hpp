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
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../components/ui.hpp"
#include "../platform/result.hpp"

#include "resource.hpp"

namespace sneze {

class texture;

class render;

/**
 * @brief font glyph
 * contains the information for a single glyph
 */
struct glyph {
    //! position of the glyph in the texture
    components::position position; // cppcheck-suppress unusedStructMember
    //! size of the glyph in the texture
    components::size size;         // cppcheck-suppress unusedStructMember
    //! offset of the glyph in the texture
    components::position offset;   // cppcheck-suppress unusedStructMember
    //! number of pixels to advance after drawing the glyph
    float advance;                 // cppcheck-suppress unusedStructMember
    //! texture page where the glyph is located
    int page;                      // cppcheck-suppress unusedStructMember

    /**
     * @brief check if the glyph is valid
     * @param glyph glyph to check
     * @return true if the glyph is valid, false otherwise
     */
    [[nodiscard]] static inline constexpr auto valid(const glyph &glyph) -> bool {
        return glyph.size.width > 0 && glyph.size.height > 0;
    }
};

/**
 * @brief font resource
 * contains the information for a font
 * @see resource
 */
class font: resource<> {
public:
    /**
     * @brief font constructor
     * @param render render to use
     */
    explicit font(class render *render): resource(render){};

    /**
     * @brief font destructor
     */
    ~font() override;

    font(const font &) = delete;
    font(font &&) = delete;
    auto operator=(const font &) -> font & = delete;
    auto operator=(font &&) -> font & = delete;

    /**
     * @brief initialize the font
     * @param file font file
     * @return result of the operation
     */
    [[nodiscard]] auto init(const std::string &file) -> result<> override;

    /**
     * @brief end the font
     */
    void end() override;

    /**
     * @brief draw text
     * @param text text to draw
     * @param position position of the text
     * @param alignment alignment of the text
     * @param size font size of the text
     * @param color color of the text
     */
    void draw_text(const std::string &text,
                   const components::position &position,
                   const components::alignment &alignment,
                   float size,
                   const components::color &color);

    /**
     * @brief get the size of the text
     * @param text text to get the size
     * @param size font size of the text
     * @return size of the text
     */
    [[nodiscard]] auto size(const std::string &text, const float &size) const -> components::size;

private:
    //! max number of page textures
    static constexpr auto max_pages = 16;
    //! params type for the tokens
    using params = std::unordered_map<std::string, std::string>;
    //! glyphs type
    using glyphs = std::array<glyph, 256>;
    //! pages type
    using pages = std::array<std::string, max_pages>;
    //! kernings type
    using kernings = std::array<std::array<int, 256>, 256>;

    //! font name
    std::string face_{};
    //! font directory
    std::filesystem::path font_directory_{};
    //! font glyphs
    glyphs glyphs_{};
    //! font kernings
    kernings kernings_{};
    //! line height
    int line_height_{0};
    //! font spacing
    components::position spacing_{0, 0};
    //! page textures
    pages pages_{""};

    //! token parsing status
    enum class token_parsing_status {
        in_key,
        in_value,
        in_value_quoted,
    };

    //! current token parsing status
    token_parsing_status token_parsing_current_status_ = token_parsing_status::in_key;

    //! current token parsing key
    std::string token_parsing_current_key_;
    //! current token parsing value
    std::string token_parsing_current_value_;
    //! current token parsing params
    params token_parsing_current_params_;
    //! current token parsing type
    std::string tokens_type_;

    /**
     * @brief parse tokens from a line
     * @param line line to parse
     * @return pair of the type and the params
     */
    [[nodiscard]] auto tokens(const std::string &line) -> std::pair<std::string, params>;


    /**
     * parsing a key
     * @param current_char current character
     */
    void tokens_in_key(const char &current_char);

    /**
     * parsing a value
     * @param current_char current character
     */
    void tokens_in_value(const char &current_char);

    /**
     * parsing a quoted value
     * @param current_char current character
     */
    void tokens_in_value_quoted(const char &current_char);

    /**
     * @brief parse a line
     * @param type type of the line
     * @param params params of the line
     * @return true if the line was ok, false otherwise
     */
    [[nodiscard]] auto parse_line(const std::string &type, const params &params) -> bool;

    /**
     * @brief parse info
     * @param params
     * @return if the info was ok
     */
    [[nodiscard]] auto parse_info(const params &params) -> bool;

    /**
     * @brief parse common
     * @param params
     * @return if the common was ok
     */
    [[nodiscard]] auto parse_common(const params &params) -> bool;

    /**
     * @brief parse page
     * @param params
     * @return if the page was ok
     */
    [[nodiscard]] auto parse_page(const params &params) -> bool;

    /**
     * @brief parse chars
     * @param params
     * @return if the chars was ok
     */
    [[nodiscard]] static auto parse_chars(const params &params) -> bool;

    /**
     * @brief parse char
     * @param params
     * @return if the char was ok
     */
    [[nodiscard]] auto parse_char(const params &params) -> bool;

    /**
     * @brief parse kernings
     * @param params
     * @return if the kernings was ok
     */
    [[nodiscard]] static auto parse_kernings(const params &params) -> bool;

    /**
     * @brief parse kerning
     * @param params
     * @return if the kerning was ok
     */
    [[nodiscard]] auto parse_kerning(const params &params) -> bool;

    /**
     * @brief get a value from the params
     * @param params params to get the value from
     * @param key key of the value
     * @return value
     */
    [[nodiscard]] static inline auto get_value(const params &params, const std::string &key) -> std::string;

    /**
     * @brief get an int from the params
     * @param params params to get the int from
     * @param key key of the int
     * @return int
     */
    [[nodiscard]] static inline auto get_int(const params &params, const std::string &key) -> int;

    /**
     * @brief get a pair of ints from the params
     * @param params params to get the pair of ints from
     * @param key key of the pair of ints
     * @return pair of ints
     */
    [[nodiscard]] static inline auto get_pair(const params &params, const std::string &key) -> std::pair<int, int>;

    /**
     * @brief validate the parsing
     * @return true if the parsing was ok, false otherwise
     */
    [[nodiscard]] auto validate_parsing() -> bool;
};

} // namespace sneze
