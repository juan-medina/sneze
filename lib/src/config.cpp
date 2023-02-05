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

#include "config.h"
#include <filesystem>
#include <fstream>
#include <utility>
#include "logger.h"
#include "platform_folders.h"

namespace sneze {

config::config(std::string team, std::string application)
    : team_{std::move(team)}, application_{std::move(application)} {
}

namespace fs = std::filesystem;

result<bool, error> config::read() {
    LOG_DEBUG("Reading config for application: {} (Team: {})", application_, team_);

    RETURN_ERR_IF_RESULT(calculate_config_file_path(), "can't calculate config file path")

    LOG_DEBUG("Config file: {}", config_file_path().generic_string());

    return true;
}

result<bool, error> config::calculate_config_file_path() {
    auto home = sago::getConfigHome();

    // home path
    fs::path home_folder(home);
    RETURN_ERR_IF(!fs::exists(home_folder), "can not find home path: {}", home)

    // team path
    fs::path team_path(team_);
    fs::path team_full_path = home_folder / team_path;
    RETURN_ERR_IF_RESULT(exist_or_create_directory(team_full_path), "error checking team path: {}", team_full_path.generic_string())

    // application path
    fs::path application_path(application_);
    fs::path application_full_path = team_full_path / application_path;
    RETURN_ERR_IF_RESULT(exist_or_create_directory(application_full_path), "error checking application path: {}", team_full_path.generic_string())

    // config file
    fs::path config_file(CONFIG_FILE_NAME);
    fs::path config_file_full_path = application_full_path / config_file;
    RETURN_ERR_IF_RESULT(exist_or_create_file(config_file_full_path), "error checking config file: {}", config_file_full_path.generic_string())
    config_file_path_ = config_file_full_path;

    return true;
}

result<bool, error> config::exist_or_create_directory(const std::filesystem::path &path) noexcept {
    if(fs::exists(path)) {
        LOG_WARN("directory already exist: {}", path.generic_string());
    } else {
        LOG_DEBUG("Creating directory: {}", path.generic_string());
        std::error_code ec;
        fs::create_directory(path, ec);
        std::error_condition ok;
        RETURN_ERR_IF(ec != ok, "error creating directory: {}", ec.message())
        RETURN_ERR_IF(!fs::exists(path), "directory does no exists after creating: {}", path.generic_string())
    }

    return true;
}
result<bool, error> config::exist_or_create_file(const std::filesystem::path &path) noexcept {
    if(fs::exists(path)) {
        LOG_WARN("file already exist: {}", path.generic_string());
    } else {
        LOG_DEBUG("Creating empty file: {}", path.generic_string());
        std::fstream file;
        file.open(path, std::ios::out);
        RETURN_ERR_IF(file.fail(), "failed to open file: {}", path.generic_string())
        file.close();
        RETURN_ERR_IF(file.fail(), "failed to close file: {}", path.generic_string())
        RETURN_ERR_IF(!fs::exists(path), "file does no exists after creating: {}", path.generic_string())
    }
    return true;
}

} // namespace sneze