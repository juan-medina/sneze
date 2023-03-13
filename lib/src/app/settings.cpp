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

#include "sneze/app/settings.hpp"

#include "sneze/platform/version.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <regex>
#include <utility>

#include <platform_folders.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace sneze {

settings::settings(std::string team, std::string application)
    : team_{std::move(team)}, application_{std::move(application)} {}

namespace fs = std::filesystem;

auto settings::read() -> result<> {
    logger::trace("reading settings for application: {} (Team: {})", application_, team_);

    if(auto [val, err] = calculate_settings_file_path().ok(); !err) {
        logger::trace("settings file: {}", val->string()); // NOLINT(bugprone-unchecked-optional-access)
        settings_file_path_ = *val;                        // NOLINT(bugprone-unchecked-optional-access)
    } else {
        logger::error("error calculate settings file path");
        return error("Can't calculate settings file path.", *err); // NOLINT(bugprone-unchecked-optional-access)
    }

    if(auto err = read_json().ko()) {
        logger::error("error reading json file: {}", settings_file_path_.string());
        return error("Can't read settings file.", *err); // NOLINT(bugprone-unchecked-optional-access)
    }

    logger::trace("settings file read");
    return true;
}

auto settings::calculate_settings_file_path() -> result<std::filesystem::path> {
    auto simple_team = simplify_name(team_);
    auto simple_application = simplify_name(application_);
    if(simple_team.empty() || simple_application.empty()) {
        logger::error("error team and application need to have valid alphanumeric characters or whitespace");
        return error("Error getting the path for settings file.");
    }

    auto home = sago::getConfigHome();

    // home path
    fs::path const home_folder{home};
    if(!fs::exists(home_folder)) {
        logger::error("error finding home path: {}", home);
        return error("Can't get game settings directory.");
    }

    // team path
    fs::path const team_path{simple_team};
    fs::path const team_full_path = home_folder / team_path;
    if(auto err = exist_or_create_directory(team_full_path).ko()) {
        logger::error("error checking team path: {}", team_full_path.string());
        return error("Can't get game settings directory.", *err);
    }

    // application path
    fs::path const application_path{simple_application};
    fs::path const application_full_path = team_full_path / application_path;
    if(auto err = exist_or_create_directory(application_full_path).ko()) {
        logger::error("error checking application path: {}", team_full_path.string());
        return error("Can't get game settings directory.", *err);
    }

    // settings file
    fs::path const settings_file_full_path = application_full_path / settings_file_name;
    if(auto err = exist_or_create_file(settings_file_full_path).ko()) {
        logger::error("error checking settings file: {}", settings_file_full_path.string());
        return error("Can't find or create settings file.", *err);
    }

    return settings_file_full_path;
}

auto settings::exist_or_create_directory(const std::filesystem::path &path) -> result<> {
    if(fs::exists(path)) {
        logger::trace("directory already exist: {}", path.string());
    } else {
        logger::trace("Creating directory: {}", path.string());
        std::error_code error_code;
        if(fs::create_directory(path, error_code); error_code) {
            logger::error("error creating directory: {}", error_code.message());
            return error("Can't get settings directory.");
        }
        if(!fs::exists(path)) {
            logger::error("directory does no exists after creating: {}", path.string());
            return error("Can't get settings directory.");
        }
    }
    return true;
}

auto settings::exist_or_create_file(const std::filesystem::path &path) -> result<> {
    if(fs::exists(path)) {
        logger::trace("file already exist: {}", path.string());
    } else {
        logger::trace("Creating empty file: {}", path.string());
        std::fstream file;
        file.open(path, std::ios::out);
        if(file.fail()) {
            logger::error("failed to open file: {}", path.string());
            return error("Can't create empty settings file.");
        }
        file.close();
        if(file.fail()) {
            logger::error("failed to close file: {}", path.string());
            return error("Can't create empty settings file.");
        }
        if(!fs::exists(path)) {
            logger::error("file does no exists after creating: {}", path.string());
            return error("Can't create empty settings file.");
        }
    }
    return true;
}

auto settings::read_json() -> result<> {
    logger::info("reading settings : {}", settings_file_path_.string());

    if(fs::file_size(settings_file_path_) == 0) {
        logger::trace("settings file is empty: {}", settings_file_path_.string());
        return true;
    }

    auto document = rapidjson::Document{};
    std::ifstream file{settings_file_path_};
    if(!file.is_open()) {
        logger::error("error opening settings file: {}", settings_file_path_.string());
        return error("Can't open settings file.");
    }

    const std::string json_string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    if(document.Parse(json_string.c_str()).HasParseError()) {
        logger::error("error parsing json file: {}", settings_file_path_.string());
        file.close();
        return error("Can't parse settings file.");
    }
    file.close();

    if(!document.IsObject()) {
        logger::error("error parsing json file: {}", settings_file_path_.string());
        return error("Can't parse settings file.");
    }

    for(const auto &section: document.GetObject()) {
        if(!section.value.IsObject()) {
            logger::error("error parsing json file: {}", settings_file_path_.string());
            return error("Can't parse settings file.");
        }
        for(const auto &name: section.value.GetObject()) {
            const auto section_name = std::string(section.name.GetString());
            const auto value_name = std::string(name.name.GetString());
            if(section_name == "sneze") {
                continue;
            }
            switch(name.value.GetType()) {
            case rapidjson::Type::kFalseType:
                data_[section_name][value_name] = false;
                break;
            case rapidjson::Type::kTrueType:
                data_[section_name][value_name] = true;
                break;
            case rapidjson::Type::kNumberType:
                if(name.value.IsInt() || name.value.IsUint() || name.value.IsInt64() || name.value.IsUint64()) {
                    data_[section_name][value_name] = std::int64_t{name.value.GetInt()};
                    break;
                }
                data_[section_name][value_name] = name.value.GetDouble();
                break;
            case rapidjson::Type::kStringType:
                data_[section_name][value_name] = std::string(name.value.GetString());
                break;
            default:
                logger::error("error invalid value type in json file: {}", settings_file_path_.string());
                return error("Can't parse settings file.");
            }
        }
    }

    return true;
}

auto settings::save() -> result<> {
    logger::info("Saving settings to: {}", settings_file_path_.string());

    auto string_buffer = rapidjson::StringBuffer();
    auto writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>(string_buffer);
    writer.StartObject();

    writer.Key("sneze");
    writer.StartObject();
    writer.Key("version");
    writer.String(version::string.c_str());
    writer.EndObject();

    for(auto &[section_name, table]: data_) {
        writer.Key(section_name.c_str());
        writer.StartObject();
        for(auto &[value_name, value]: table) {
            writer.Key(value_name.c_str());
            if(std::holds_alternative<bool>(value)) {
                writer.Bool(std::get<bool>(value));
            } else if(std::holds_alternative<double>(value)) {
                writer.Double(std::get<double>(value));
            } else if(std::holds_alternative<std::int64_t>(value)) {
                writer.Int64(std::get<std::int64_t>(value));
            } else if(std::holds_alternative<std::string>(value)) {
                writer.String(std::get<std::string>(value).c_str());
            }
        }
        writer.EndObject();
    }
    writer.EndObject();
    std::ofstream file;
    file.open(settings_file_path_);
    if(file.fail()) {
        logger::error("failed to open file: {}", settings_file_path_.string());
        return error("Can't save settings file.");
    }
    file << string_buffer.GetString();
    if(file.fail()) {
        logger::error("failed to write file: {}", settings_file_path_.string());
        file.close();
        return error("Can't save settings file.");
    }
    file.close();
    if(file.fail()) {
        logger::error("failed to close file: {}", settings_file_path_.string());
        return error("Can't save settings file.");
    }
    logger::trace("Settings saved");
    return true;
}

auto settings::simplify_name(const std::string &name) -> std::string {
    const std::string result = std::regex_replace(name, std::regex{R"([\s])"}, "_");
    return std::regex_replace(result, std::regex{R"([^\w])"}, "");
}

} // namespace sneze
