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

#include <cstdint>
#include <filesystem>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../platform/logger.hpp"
#include "../platform/result.hpp"

namespace sneze {

/**
 * @brief Concept for value types
 *
 * This is a concept to check if a type is a valid value type for settings
 *
 * @note Valid types are:
 * - std::int64_t
 * - double
 * - bool
 * - std::string
 *
 * @tparam Type
 */
template<typename Type>
concept is_value = std::is_same_v<Type, std::int64_t> || std::is_same_v<Type, double> || std::is_same_v<Type, bool>
                   || std::is_same_v<Type, std::string>;

/**
 * @brief Application settings
 *
 * This class is used to store application settings, it is used by the application class.
 *
 * The settings file is located in the user's configuration folder, this folder is different for each platform:
 *
 * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
 * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
 * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
 *
 * The file name is simplified, so does not contain invalid characters for the file system:
 * - Whitespaces are replaced with underscores
 * - All the non alphanumeric characters are removed
 *
 * The settings file is a JSON file, the format is:
 * @code
 * {
 *  "sneze": {
 *    "version": "v.MAJOR.MINOR.RELEASE"
 *  },
 *  "section_name": {
 *    "value": "setting_value",
 *  },
 * }
 * @endcode
 */
class settings {
public:
    /**
     * @brief Construct a new settings object
     *
     * @param team The team name
     * @param application The application name
     */
    settings(std::string team, std::string application);

    /**
     * @brief Read the settings file
     *
     * @return result<>
     *
     * @note If the file does not exist, it will be created
     *
     * @see write()
     * @see sneze::result
     */
    auto read() -> result<>;

    /**
     * @brief Write the settings file
     *
     * @return result<>
     *
     * @note If the file does not exist, it will be created
     *
     * @see read()
     * @see sneze::result
     */
    auto save() -> result<>;

    /**
     * @brief Set a setting value
     *
     * @tparam Type The type of the value
     * @param section The section name
     * @param name The setting name
     * @param value The value
     *
     * @note If the section does not exist, it will be created
     */
    template<is_value Type>
    inline void set(const std::string &section, const std::string &name, const Type &value) {
        if(auto it_section = data_.find(section); it_section != data_.end()) {
            it_section->second[name] = value;
        } else {
            auto new_section = settings::section{};
            new_section[name] = value;
            data_[section] = new_section;
        }
    }

    /**
     * @brief Get a setting value
     * @tparam Type the type of the value
     * @param section the section name
     * @param name the setting name
     * @param default_value the default value to return if the setting is not found
     * @return the setting value, or the default value if the setting is not found
     */
    template<is_value Type>
    inline auto get(const std::string &section, const std::string &name, const Type &default_value) {
        if(auto it_section = data_.find(section); it_section != data_.end()) {
            if(auto it_value = it_section->second.find(name); it_value != it_section->second.end()) {
                if(auto value = std::get_if<Type>(&it_value->second)) {
                    return *value;
                }
            }
        }
        logger::trace("setting value not found, section: {}, value: {}, default to {}", section, name, default_value);
        set<Type>(section, name, default_value);
        return default_value;
    }

private:
    //! setting type
    using settings_value = std::variant<std::int64_t, double, bool, std::string>;

    //! section type
    using section = std::unordered_map<std::string, settings_value>;

    //! sections type
    using sections = std::unordered_map<std::string, section>;

    //! the settings data
    sections data_;

    //! the settings file path
    std::filesystem::path settings_file_path_;

    //! the team name
    std::string team_;

    //! the application name
    std::string application_;

    //! the settings default file name
    constexpr static const auto settings_file_name = "settings.json";

    /**
     * @brief Check if a directory exists, if not, create it
     *
     * @param path The path to the directory
     * @return result<>
     *
     * @see sneze::result
     */
    static auto exist_or_create_directory(const std::filesystem::path &path) -> result<>;

    /**
     * @brief Check if a file exists, if not, create it
     *
     * @param path The path to the file
     * @return result<>
     *
     * @see sneze::result
     */
    static auto exist_or_create_file(const std::filesystem::path &path) -> result<>;

    /**
     * @brief Get the settings file path
     *
     * @note The settings file is located in the user's configuration folder, this folder is different for each
     * platform:
     * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
     * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
     * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
     *
     * The file name is simplified, so does not contain invalid characters for the file system:
     * - Whitespaces are replaced with underscores
     * - All the non alphanumeric characters are removed
     *
     * @return result<std::filesystem::path>
     *
     * @see sneze::result
     *
     * @return result<std::filesystem::path>
     *
     * @see sneze::result
     */
    auto calculate_settings_file_path() -> result<std::filesystem::path>;

    /**
     * @brief Read the settings file in json format
     *
     * @return result<>
     *
     * @see sneze::result
     */
    auto read_json() -> result<>;

    /**
     * @brief simplify a name
     *
     * This function is used to simplify a name, either team or application, it is used to create the settings
     * file path. Whitespace characters are replaced by underscores, non alphanumeric characters are removed.
     *
     * @param name the name to simplify
     * @return the simplified name
     * @see calculate_settings_file_path()
     */
    static auto simplify_name(const std::string &name) -> std::string;
};

} // namespace sneze
