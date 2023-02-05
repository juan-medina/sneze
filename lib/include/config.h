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

// https://github.com/ToruNiina/toml11

#include <filesystem>
#include <string>
#include "result.h"

namespace sneze {
class config {
public:
    config(std::string team, std::string application); // NOLINT(google-explicit-constructor)
    virtual ~config() = default;

    result<bool, error> read();

protected:
    std::string team_;
    std::string application_;

private:
    static result<bool, error> exist_or_create_directory(const std::filesystem::path &path) noexcept;
    static result<bool, error> exist_or_create_file(const std::filesystem::path &path) noexcept;

    constexpr static const char *const CONFIG_FILE_NAME = "config.toml";
    result<bool, error> calculate_config_file_path();

    [[nodiscard]] inline const std::filesystem::path &config_file_path() const noexcept {
        return config_file_path_;
    }

    std::filesystem::path config_file_path_;
};
} // namespace sneze
