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

#include "spdlog/spdlog.h"

namespace sneze::logger {

namespace level {
enum log_level : int {
    trace = spdlog::level::trace,
    debug = spdlog::level::debug,
    info = spdlog::level::info,
    warn = spdlog::level::warn,
    err = spdlog::level::err,
    critical = spdlog::level::critical,
    off = spdlog::level::off
};
} // namespace level

#if defined(NDEBUG)
template<typename... Args>
static void log(Args &&...) {}
#else
template<typename... Args>
static void log(const level::log_level level, std::source_location source, fmt::format_string<Args...> fmt,
                Args &&...args) {
    auto location = spdlog::source_loc{source.file_name(), static_cast<int>(source.line()), source.function_name()};
    spdlog::log(location, static_cast<spdlog::level::level_enum>(level), fmt, std::forward<Args>(args)...);
}
#endif

template<typename... Args>
struct info {
    explicit info(fmt::format_string<Args...> fmt, Args &&...args,
                  const std::source_location &location = std::source_location::current()) {
        log(level::info, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
info(fmt::format_string<Args...> fmt, Args &&...) -> info<Args...>;

template<typename... Args>
struct debug {
    explicit debug(fmt::format_string<Args...> fmt, Args &&...args,
                   const std::source_location &location = std::source_location::current()) {
        log(level::debug, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
debug(fmt::format_string<Args...> fmt, Args &&...) -> debug<Args...>;

template<typename... Args>
struct error {
    explicit error(fmt::format_string<Args...> fmt, Args &&...args,
                   const std::source_location &location = std::source_location::current()) {
        log(level::err, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
error(fmt::format_string<Args...> fmt, Args &&...) -> error<Args...>;

template<typename... Args>
struct warning {
    explicit warning(fmt::format_string<Args...> fmt, Args &&...args,
                     const std::source_location &location = std::source_location::current()) {
        log(level::warn, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
warning(fmt::format_string<Args...> fmt, Args &&...) -> warning<Args...>;

void setup_log() noexcept;

void set_log_level(level::log_level level) noexcept;

} // namespace sneze::logger
