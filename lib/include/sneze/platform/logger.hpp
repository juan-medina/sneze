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

#include <source_location>
#include <string>

#include <spdlog/spdlog.h>

namespace sneze::logger {

namespace level {
enum log_level : int {
    trace = spdlog::level::trace,
    debug = spdlog::level::debug,
    info = spdlog::level::info,
    warning = spdlog::level::warn,
    error = spdlog::level::err,
    critical = spdlog::level::critical,
    off = spdlog::level::off
};
} // namespace level

template<typename... Args>
static void
log(const level::log_level level, std::source_location source, fmt::format_string<Args...> fmt, Args &&...args) {
    auto location = spdlog::source_loc{source.file_name(), static_cast<int>(source.line()), source.function_name()};
    spdlog::log(location, static_cast<spdlog::level::level_enum>(level), fmt, std::forward<Args>(args)...);
}

template<typename... Args>
struct info {
    explicit info(fmt::format_string<Args...> fmt,
                  Args &&...args,
                  const std::source_location &location = std::source_location::current()) {
        log(level::info, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
info(fmt::format_string<Args...> fmt, Args &&...) -> info<Args...>;

template<typename... Args>
struct debug {
    explicit debug(fmt::format_string<Args...> fmt,
                   Args &&...args,
                   const std::source_location &location = std::source_location::current()) {
        log(level::debug, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
debug(fmt::format_string<Args...> fmt, Args &&...) -> debug<Args...>;

template<typename... Args>
struct error {
    explicit error(fmt::format_string<Args...> fmt,
                   Args &&...args,
                   const std::source_location &location = std::source_location::current()) {
        log(level::error, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
error(fmt::format_string<Args...> fmt, Args &&...) -> error<Args...>;

template<typename... Args>
struct warning {
    explicit warning(fmt::format_string<Args...> fmt,
                     Args &&...args,
                     const std::source_location &location = std::source_location::current()) {
        log(level::warning, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
warning(fmt::format_string<Args...> fmt, Args &&...) -> warning<Args...>;

void setup_log();

void set_level(level::log_level level);

auto level_from_string(const std::string &level) -> level::log_level;

auto string_from_level(level::log_level level) -> std::string;

} // namespace sneze::logger
