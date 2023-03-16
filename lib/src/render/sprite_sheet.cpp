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

#include "sneze/render/sprite_sheet.hpp"

#include "sneze/render/render.hpp"

#include <fstream>

#include <rapidjson/document.h>

namespace fs = std::filesystem;

namespace sneze {

sprite_sheet::~sprite_sheet() {
    sprite_sheet::end();
}

auto parse_meta_data(const rapidjson::Document &document, const fs::path &base_dir) -> result<std::string, error> {
    const auto &meta_value = document["meta"];
    if(!meta_value.IsObject()) {
        logger::error("error parsing sprite sheet, meta not found");
        return error("Can't parse sprite sheet file.");
    }

    const auto *texture_path = meta_value["image"].GetString();
    if(texture_path == nullptr) {
        logger::error("error parsing sprite sheet, no texture path");
        return error("Can't parse sprite sheet file.");
    }

    auto full_texture_path = fs::path{base_dir} / std::string(texture_path);
    return full_texture_path.string();
}

auto parse_frames(const rapidjson::Document &document, std::unordered_map<std::string, frame> &frames_map) -> result<> {
    const auto &frames = document["frames"];
    if(!frames.IsArray()) {
        logger::error("error parsing sprite sheet, does not have frames");
        return error("Can't parse sprite sheet file.");
    }

    for(rapidjson::SizeType i = 0; i < frames.Size(); i++) {
        const auto &frame_object = frames[i];
        if(!frame_object.IsObject()) {
            logger::error("error parsing sprite sheet, invalid frame");
            return error("Can't parse sprite sheet file.");
        }

        const auto &frame_name = frame_object["filename"].GetString();
        if(frame_name == nullptr) {
            logger::error("error parsing sprite sheet, frame name is null");
            return error("Can't parse sprite sheet file.");
        }

        const auto &frame_data = frame_object["frame"];
        if(!frame_data.IsObject()) {
            logger::error("error parsing sprite sheet, frame data not present in frame");
            return error("Can't parse sprite sheet file.");
        }

        const auto &frame_x = frame_data["x"].GetFloat();
        const auto &frame_y = frame_data["y"].GetFloat();
        const auto &frame_w = frame_data["w"].GetFloat();
        const auto &frame_h = frame_data["h"].GetFloat();
        auto rect = components::rect{{frame_x, frame_y}, {frame_w, frame_h}};

        const auto &pivot_data = frame_object["pivot"];
        if(!pivot_data.IsObject()) {
            logger::error("error parsing sprite sheet, pivot data no present in frame");
            return error("Can't parse sprite sheet file.");
        }

        const auto &pivot_x = pivot_data["x"].GetFloat();
        const auto &pivot_y = pivot_data["y"].GetFloat();
        const auto pivot = components::position{pivot_x, pivot_y};

        frames_map.emplace(frame_name, frame{rect, pivot});
    }

    return true;
}

auto sprite_sheet::init(const std::string &uri, bool is_single_texture) -> result<> {
    logger::trace("sprite sheet init");

    if(const auto file_path = fs::path{uri}; fs::exists(file_path)) {
        sprite_sheet_directory_ = file_path.parent_path();

        if(is_single_texture) {
            return init_from_texture(file_path);
        }
        return init_from_json(file_path);
    }

    logger::error("error sprite sheet does not exist: {}", uri);
    return error("Error sprite sheet not exist.");
}

auto sprite_sheet::init_from_json(const std::filesystem::path &file_path) -> result<> {
    std::ifstream file{file_path};
    if(!file.is_open()) {
        logger::error("error opening sprite sheet file: {}", file_path.string());
        return error("Can't open sprite sheet file.");
    }

    auto document = rapidjson::Document{};
    const std::string json_string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    if(document.Parse(json_string.c_str()).HasParseError()) {
        logger::error("error parsing json file: {}", file_path.string());
        file.close();
        return error("Can't parse sprite sheet file.");
    }
    file.close();

    if(!document.IsObject()) {
        logger::error("error parsing json file: {}", file_path.string());
        return error("Can't parse sprite sheet file.");
    }

    if(auto err = parse_frames(document, frames_).ko(); err) {
        logger::error("error parsing frames");
        return error("Can't parse sprite sheet file.", *err);
    }

    if(auto [texture_name, err] = parse_meta_data(document, sprite_sheet_directory_).ok(); !err) {
        texture_ = *texture_name;
    } else {
        logger::error("error parsing meta data");
        return error("Can't parse sprite sheet file.", *err);
    }

    if(auto err = get_render()->load_texture(texture_).ko(); err) {
        logger::error("error loading sprite sheet texture: {}", texture_);
        texture_ = "";
        return error("Can't load texture.", *err);
    }

    logger::trace("sprite sheet init success");

    for(const auto &[name, frame]: frames_) {
        logger::trace("  frame: {} size: {}x{}", name, frame.rect.size.width, frame.rect.size.height);
    }

    return true;
}

auto sprite_sheet::init_from_texture(const std::filesystem::path &file_path) -> result<> {
    texture_ = file_path.string();
    if(auto err = get_render()->load_texture(texture_).ko(); err) {
        logger::error("error loading sprite sheet from texture: {}", texture_);
        texture_ = "";
        return error("Can't load texture.", *err);
    }

    auto texture = get_render()->get_texture(texture_);

    auto new_frame = frame{};

    new_frame.rect.position = {0, 0};
    new_frame.rect.size = texture->size();
    new_frame.pivot = {0.5F, 0.5F};
    frames_.emplace("default", new_frame);

    logger::trace("sprite sheet init success");

    return true;
}

auto sprite_sheet::end() -> void {
    logger::trace("sprite sheet end");
    if(!texture_.empty()) {
        get_render()->unload_texture(texture_);
        texture_.clear();
    }
    frames_.clear();

    sprite_sheet_directory_.clear();
}

void sprite_sheet::draw_sprite(const std::string &name,
                               const components::position &position,
                               const bool &flip_x,
                               const bool &flip_y,
                               const float &rotation,
                               const float &scale,
                               const components::color &color) const {
    if(auto it_frame = frames_.find(name); it_frame != frames_.end()) [[likely]] {
        const auto &frame = it_frame->second;

        auto texture = get_render()->get_texture(texture_);

        if(texture == nullptr) {
            logger::error("error drawing sprite, texture not found: {}", texture_);
            return;
        }

        auto pos_x = frame.rect.size.width * frame.pivot.x;
        auto pos_y = frame.rect.size.height * frame.pivot.y;

        auto dest = components::rect{{position.x - (pos_x * scale), position.y - (pos_y * scale)},
                                     {frame.rect.size.width * scale, frame.rect.size.height * scale}};

        texture->draw(frame.rect, dest, flip_x, flip_y, rotation, color);
    } else {
        logger::error("error drawing sprite, frame not found: {}", name);
    }
}

} // namespace sneze
