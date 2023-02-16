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

#include "sneze/platform/logger.hpp"
#include "sneze/platform/result.hpp"

#include <filesystem>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include <toml.hpp>

namespace sneze {

template<typename T>
concept is_value = std::is_same_v<T, std::int64_t> || std::is_same_v<T, double> || std::is_same_v<T, bool>
                   || std::is_same_v<T, std::string>;

class settings {
public:
    settings(std::string team, std::string application);

    auto read() -> result<>;

    using settings_value = std::variant<std::int64_t, double, bool, std::string>;

    template<is_value Type>
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    inline void set(const std::string &section, const std::string &name, const Type &value) {
        if(auto it_section = data_.find(section); it_section != data_.end()) {
            it_section->second[name] = value;
        } else {
            auto new_section = settings::section{};
            new_section[name] = value;
            data_[section] = new_section;
        }
    }

    template<is_value Type>
    inline auto get(const std::string &section, const std::string &name, const Type &default_value) {
        if(auto it_section = data_.find(section); it_section != data_.end()) {
            if(auto it_value = it_section->second.find(name); it_value != it_section->second.end()) {
                if(auto value = std::get_if<Type>(&it_value->second)) {
                    return *value;
                }
            }
        }
        logger::debug("setting value not found, section: {}, value: {}, default to {}", section, name, default_value);
        set<Type>(section, name, default_value);
        return default_value;
    }

    auto save() -> result<>;

private:
    std::string application_;
    std::string team_;

    constexpr static const auto Settings_file_name = "settings.toml";

    static auto exist_or_create_directory(const std::filesystem::path &path) -> result<>;

    static auto exist_or_create_file(const std::filesystem::path &path) -> result<>;

    auto calculate_settings_file_path() -> result<std::filesystem::path>;

    auto read_toml() -> result<>;

    using section = std::unordered_map<std::string, settings_value>;
    using sections = std::unordered_map<std::string, section>;

    sections data_;
    std::filesystem::path settings_file_path_;

    auto add_toml_value(const std::string &section, const std::string &name, const toml::value &value) -> result<>;
};

} // namespace sneze
