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

#include "sneze/render/font.hpp"

#include "sneze/render/render.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace sneze {

auto font::init(const std::string &file) -> result<> {
    if(const auto file_path = fs::path{file}; fs::exists(file_path)) {
        font_directory_ = file_path.parent_path();

        auto stream = std::ifstream{file_path};
        if(!stream.is_open()) {
            logger::error("error opening font file: {}", file);
            return error{"Error opening font file"};
        }
        auto line = std::string{};
        while(!stream.eof()) {
            std::getline(stream, line);
            auto [type, params] = font::tokens(line);
            if(!parse_line(type, params)) {
                logger::error("error parsing line in font line: {}", line);
                stream.close();
                return error{"Error in font format."};
            }
        }

        if(!validate_parsing()) {
            logger::error("error parsing font file");
            return error{"Error in font format."};
        }
        logger::info("font: \"{}\" loaded correctly", face_);
        return true;
    }
    logger::error("error font does not exist: {}", file);
    return error("Error font does not exist.");
}

void font::end() {
    logger::info("unload font: {}", face_);

    for(const auto &page: pages_) {
        if(!page.empty()) {
            get_render()->unload_texture(page);
        }
    }

    pages_ = {};
    glyphs_ = {};
    kernings_ = {};
    line_height_ = {0};
    glyphs_ = {};
    spacing_ = {0, 0};
    pages_ = {};
    font_directory_ = std::filesystem::path{""};
    kernings_ = {};
}

auto font::tokens(const std::string &line) -> std::pair<std::string, params> {
    token_parsing_current_status_ = token_parsing_status::in_key;
    tokens_type_ = "";
    token_parsing_current_key_ = "";
    token_parsing_current_value_ = "";
    token_parsing_current_params_ = {};

    for(const auto &current_char: line) {
        switch(token_parsing_current_status_) {
        case token_parsing_status::in_key:
            tokens_in_key(current_char);
            break;
        case token_parsing_status::in_value:
            tokens_in_value(current_char);
            break;
        case token_parsing_status::in_value_quoted:
            tokens_in_value_quoted(current_char);
            break;
        }
    }

    if(token_parsing_current_status_ == token_parsing_status::in_value) {
        token_parsing_current_params_.insert({token_parsing_current_key_, token_parsing_current_value_});
    }
    return std::make_pair(tokens_type_, token_parsing_current_params_);
}

void font::tokens_in_key(const char &current_char) {
    if(current_char == '=') {
        token_parsing_current_status_ = token_parsing_status::in_value;
    } else if(current_char == ' ') {
        token_parsing_current_status_ = token_parsing_status::in_key;
        if(!token_parsing_current_key_.empty()) {
            if(tokens_type_.empty()) {
                tokens_type_ = token_parsing_current_key_;
            } else {
                token_parsing_current_params_.clear();
                return;
            }
            token_parsing_current_key_.clear();
        }
    } else {
        token_parsing_current_key_ += current_char;
    }
}

void font::tokens_in_value(const char &current_char) {
    if(current_char == '"') {
        token_parsing_current_status_ = token_parsing_status::in_value_quoted;
    } else if(current_char == ' ') {
        token_parsing_current_params_.insert({token_parsing_current_key_, token_parsing_current_value_});
        token_parsing_current_key_.clear();
        token_parsing_current_value_.clear();
        token_parsing_current_status_ = token_parsing_status::in_key;
    } else {
        token_parsing_current_value_ += current_char;
    }
}

void font::tokens_in_value_quoted(const char &current_char) {
    if(current_char == '"') {
        token_parsing_current_params_.insert({token_parsing_current_key_, token_parsing_current_value_});
        token_parsing_current_key_.clear();
        token_parsing_current_value_.clear();
        token_parsing_current_status_ = token_parsing_status::in_key;
    } else {
        token_parsing_current_value_ += current_char;
    }
}

auto font::parse_line(const std::string &type, const params &params) -> bool {
    if(type == "info") {
        return parse_info(params);
    } else if(type == "common") { // NOLINT(readability-else-after-return)
        return parse_common(params);
    } else if(type == "page") {
        return parse_page(params);
    } else if(type == "chars") {
        return parse_chars(params);
    } else if(type == "char") {
        return parse_char(params);
    } else if(type == "kernings") {
        return parse_kernings(params);
    } else if(type == "kerning") {
        return parse_kerning(params);
    } else if(type.empty()) {
        return true;
    } else {
        logger::error("error parsing font file: invalid line type: {}", type);
        return false;
    }
}

auto font::parse_info(const font::params &params) -> bool {
    face_ = get_value(params, "face");
    auto [spacing_x, spacing_y] = get_pair(params, "spacing");
    if((spacing_x < 0) || (spacing_y < 0)) {
        logger::error("error parsing font file: invalid spacing: {} {}", spacing_x, spacing_y);
        return false;
    }
    spacing_ = {static_cast<float>(spacing_x), static_cast<float>(spacing_y)};

    return true;
}

auto font::parse_common(const params &params) -> bool {
    line_height_ = get_int(params, "lineHeight");

    if(line_height_ == 0) {
        logger::error("error parsing font file: invalid line height");
        return false;
    }

    return true;
}

auto font::parse_page(const params &params) -> bool {
    const auto page_id = get_int(params, "id");
    if(page_id < 0 || page_id >= max_pages) {
        logger::error("error parsing font file: invalid page id: {}, constrains: 0-{}", page_id, max_pages);
        return false;
    }
    const auto file = get_value(params, "file");
    if(file.empty()) {
        logger::error("error parsing font file: invalid page file");
        return false;
    }

    bool result = true;

    auto file_path = (font_directory_ / file).string();

    if(get_render()->load_texture(file_path).ko()) {
        logger::error("error parsing page: can't load texture");
        result = false;
    } else {
        pages_.at(page_id) = file_path;
    }

    return result;
}

auto font::parse_chars(const params &params) -> bool {
    auto char_count = get_int(params, "count");
    if((char_count == 0) || (char_count > 256)) {
        logger::error("error parsing font file: invalid char count: {}", char_count);
        return false;
    }
    return true;
}

auto font::parse_char(const params &params) -> bool {
    auto new_glyph = glyph{};

    const auto char_id = get_int(params, "id");
    if((char_id < 0) || (char_id > 255)) {
        logger::error("error parsing font file: invalid glyph id: {}", char_id);
        return false;
    }

    const auto pos_x = static_cast<float>(get_int(params, "x"));
    const auto pos_y = static_cast<float>(get_int(params, "y"));
    new_glyph.position = {pos_x, pos_y};

    const auto width = static_cast<float>(get_int(params, "width"));
    const auto height = static_cast<float>(get_int(params, "height"));
    new_glyph.size = {width, height};

    const auto x_offset = static_cast<float>(get_int(params, "xoffset"));
    const auto y_offset = static_cast<float>(get_int(params, "yoffset"));
    new_glyph.offset = {x_offset, y_offset};

    new_glyph.advance = static_cast<float>(get_int(params, "xadvance"));

    new_glyph.page = get_int(params, "page");
    if((new_glyph.page < 0) || (new_glyph.page >= static_cast<int>(pages_.size()))) {
        logger::error("error parsing font file: invalid glyph page: {}", new_glyph.page);
        return false;
    }

    glyphs_[char_id] = new_glyph;

    return true;
}

auto font::parse_kernings(const params &params) -> bool {
    if(get_int(params, "count") == 0) {
        logger::error("error parsing font file: invalid kerning count");
        return false;
    }
    return true;
}

auto font::parse_kerning(const params &params) -> bool {
    auto first = get_int(params, "first");
    auto second = get_int(params, "second");
    if((first < 0) || (first > 255) || (second < 0) || (second > 255)) {
        logger::error("error parsing font file: invalid kerning pair: {} {}", first, second);
        return false;
    }
    kernings_.at(first).at(second) = get_int(params, "amount");
    return true;
}

auto font::get_value(const params &params, const std::string &key) -> std::string {
    if(const auto it_param = params.find(key); it_param != params.end()) {
        return it_param->second;
    }
    return "";
}

auto font::get_int(const params &params, const std::string &key) -> int {
    auto value = get_value(params, key);
    if(value.empty()) {
        return 0;
    }
    return std::stoi(value);
}

auto font::get_pair(const font::params &params, const std::string &key) -> std::pair<int, int> {
    auto value = get_value(params, key);

    if(auto comma_at = value.find(','); comma_at != std::string::npos) {
        auto first = value.substr(0, comma_at);
        auto second = value.substr(comma_at + 1);
        return {std::stoi(first), std::stoi(second)};
    }

    return {0, 0};
}

auto font::validate_parsing() -> bool {
    if(pages_.empty()) {
        logger::error("error parsing font file: no pages found");
        return false;
    }

    if(!std::any_of(glyphs_.begin(), glyphs_.end(), glyph::valid)) {
        logger::error("error parsing font file: no valid glyphs");
        return false;
    }

    return true;
}

void font::draw_text(const std::string &text,
                     const components::position &position,
                     const components::alignment &alignment,
                     float size,
                     const components::color &color) {
    auto scale_size = size / static_cast<float>(line_height_);

    components::position current_position = position;

    auto text_size = font::size(text, size);

    switch(alignment.horizontal) {
    case components::horizontal::center:
        current_position.x -= text_size.width / 2;
        break;
    case components::horizontal::right:
        current_position.x -= text_size.width;
        break;
    default:
        break;
    }

    switch(alignment.vertical) {
    case components::vertical::center:
        current_position.y -= text_size.height / 2;
        break;
    case components::vertical::bottom:
        current_position.y -= text_size.height;
        break;
    default:
        break;
    }

    unsigned char previous_char = 0;

    for(const auto &current_char: text) {
        const auto &glyph = glyphs_.at(current_char);
        if(!glyph::valid(glyph)) {
            continue;
        }
        auto texture_name = pages_.at(glyph.page);
        const auto texture = get_render()->get_texture(texture_name);

        if(texture == nullptr) {
            logger::error("error drawing text: can't find texture {}", texture_name);
            return;
        }

        using rect = components::rect;
        const auto src = rect{{glyph.position.x, glyph.position.y}, {glyph.size.width, glyph.size.height}};

        if(previous_char != 0U) {
            current_position.x += static_cast<float>(kernings_.at(previous_char).at(current_char)) * scale_size;
        }

        const auto dst = rect{
            {current_position.x + (glyph.offset.x * scale_size), current_position.y + (glyph.offset.y * scale_size)},
            {glyph.size.width * scale_size, glyph.size.height * scale_size}};

        texture->draw(src, dst, color);

        current_position.x += (glyph.advance * scale_size);
        current_position.x += (spacing_.x * scale_size);

        previous_char = current_char;
    }
}

font::~font() {
    font::end();
}

auto font::size(const std::string &text, const float &size) const -> components::size {
    auto scale_size = size / static_cast<float>(line_height_);
    unsigned char previous_char = 0;

    float advance = 0;

    for(const auto &current_character: text) {
        const auto &glyph = glyphs_.at(current_character);
        if(!glyph::valid(glyph)) {
            continue;
        }

        if(previous_char != 0U) {
            advance += static_cast<float>(kernings_.at(previous_char).at(current_character)) * scale_size;
        }

        advance += (glyph.advance * scale_size);
        advance += (spacing_.x * scale_size);
        previous_char = current_character;
    }

    return {advance, static_cast<float>(line_height_) * scale_size};
}

} // namespace sneze
