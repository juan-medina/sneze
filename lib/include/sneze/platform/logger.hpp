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

#include <string>

#if __has_include(<source_location>)
#    include <source_location>
#elif __has_include(<experimental/source_location>)
#    include <experimental/source_location>
namespace std {
using source_location = experimental::source_location;
}
#else
#    define NO_SOURCE_LOCATION
#    include <cstdint>
namespace std {
struct source_location {
    static constexpr auto current() noexcept -> source_location {
        return source_location{};
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] constexpr auto line() const noexcept -> uint_least32_t {
        return 0;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] constexpr auto column() const noexcept -> uint_least32_t {
        return 0;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] constexpr auto file_name() const noexcept -> const char * {
        return "";
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] constexpr auto function_name() const noexcept -> const char * {
        return "";
    }
};
} // namespace std
#endif

#include <spdlog/spdlog.h>

/**
 * @brief provides a simple way to log messages
 *
 * This is a simple wrapper around spdlog that provides a simple way to log messages
 * using the fmt library. The messages are logged with the file name, line number and
 * function name in which the log message was called when running in debug mode.
 *
 * @attention Logging file names, line numbers and function names is only supported
 * when the compiler supports the C++20 source_location header. Currently GCC, clang 10 & MSVC
 * support this, apple's clang does not.
 *
 * By default the logger is set to log messages with the level debug and above when running
 * in debug mode and with the level info and above when running in release mode.
 *
 * The current log level can be changed by calling the function logger::set_level and
 * will be read/write from sneze::settings.
 *
 * The log will be written to the file **sneze.log** in the current working directory, and
 * will be displayed colored in the standard output.
 *
 * @code
 * sneze::logger::info("Hello {}", "World");
 * @endcode
 */
namespace sneze::logger {

//! the different log levels
enum class level : int {
    //! log all messages
    trace = spdlog::level::trace,
    //! log debug, info, warning, error and critical messages
    debug = spdlog::level::debug,
    //! log info, warning, error and critical messages
    info = spdlog::level::info,
    //! log warning, error and critical messages
    warning = spdlog::level::warn,
    //! log error and critical messages
    error = spdlog::level::err,
    //! log critical messages
    critical = spdlog::level::critical,
    //! log no messages
    off = spdlog::level::off
};

template<typename... Args>
static void
log(const level level, std::source_location source, fmt::format_string<Args...> fmt, Args &&...args) {
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

/**
 * @brief log a message with sneze::logger::level::info
 *
 * @tparam Args the types of the arguments to be formatted
 * @param fmt the format string
 */
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

/**
 * @brief log a message with sneze::logger::level::debug
 *
 * @tparam Args the types of the arguments to be formatted
 * @param fmt the format string
 */
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

/**
 * @brief log a message with sneze::logger::level::error
 *
 * @tparam Args the types of the arguments to be formatted
 * @param fmt the format string
 */
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

/**
 * @brief log a message with sneze::logger::level::warning
 *
 * @tparam Args the types of the arguments to be formatted
 * @param fmt the format string
 */
template<typename... Args>
warning(fmt::format_string<Args...> fmt, Args &&...) -> warning<Args...>;

template<typename... Args>
struct trace {
    explicit trace(fmt::format_string<Args...> fmt,
                   Args &&...args,
                   const std::source_location &location = std::source_location::current()) {
        log(level::trace, location, fmt, std::forward<Args>(args)...);
    }
};

template<typename... Args>
trace(fmt::format_string<Args...> fmt, Args &&...) -> trace<Args...>;

//! setup the logger system
void setup_log();

/**
 * @brief set the log level
 *
 * @param level the new log sneze::logger:level
 */
void set_level(level level);

/**
 * @brief get a sneze::logger::level from a string
 * @param level_string the string representation of the sneze logger level
 * @return the sneze logger level
 */
auto level_from_string(const std::string &level_string) -> level;

/**
 * @brief get a string from a sneze::logger::level
 *
 * @param level the sneze logger level
 * @return the string representation of the sneze logger level
 */
auto string_from_level(level level) -> std::string;

} // namespace sneze::logger
