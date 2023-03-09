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

#include "sneze/platform/logger.hpp"

#include <memory>

#include <entt/core/hashed_string.hpp>
#include <SDL_log.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#    include <spdlog/sinks/msvc_sink.h>
#endif

namespace sneze::logger {

void setup_spdlog() {
    auto dist_sink = std::make_shared<spdlog::sinks::dist_sink_st>();

    auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    dist_sink->add_sink(color_sink);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("sneze.log", true);
    dist_sink->add_sink(file_sink);

#if defined(_MSC_VER) && defined(_DEBUG)
    auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
    dist_sink->add_sink(debug_sink);
#endif
#if not defined(NDEBUG) && not defined(NO_SOURCE_LOCATION)
    dist_sink->set_pattern("[%x] [%H:%M:%S.%e] [%^%-8l%$] %v -> %@");
#else
    dist_sink->set_pattern("[%x] [%H:%M:%S.%e] [%^%-8l%$] %v");
#endif
    auto logger = std::make_shared<spdlog::logger>("sneze", dist_sink);
    spdlog::set_default_logger(logger);
}

void sdl_log_callback(void * /*user_data*/, int /*category*/, SDL_LogPriority priority, const char *message) {
    level::log_level log_level; // NOLINT(cppcoreguidelines-init-variables)

    switch(priority) {
    case SDL_LOG_PRIORITY_VERBOSE:
        log_level = level::trace;
        break;
    case SDL_LOG_PRIORITY_DEBUG:
        log_level = level::debug;
        break;
    case SDL_LOG_PRIORITY_INFO:
        log_level = level::info;
        break;
    case SDL_LOG_PRIORITY_WARN:
        log_level = level::warning;
        break;
    [[likely]] case SDL_LOG_PRIORITY_ERROR:
        log_level = level::error;
        break;
    case SDL_LOG_PRIORITY_CRITICAL:
        log_level = level::critical;
        break;
    [[unlikely]] default:
        log_level = level::off;
        break;
    }
    log(log_level, std::source_location::current(), "SDL: {}", message);
}

void setup_log() {
    setup_spdlog();
    SDL_LogSetOutputFunction(sdl_log_callback, nullptr);
#ifdef NDEBUG
    set_level(logger::level::info);
#else
    set_level(logger::level::debug);
#endif
}

void set_level(level::log_level level) {
    spdlog::level::level_enum spdlog_level; // NOLINT(cppcoreguidelines-init-variables)
    SDL_LogPriority sdl_log_level;          // NOLINT(cppcoreguidelines-init-variables)

    switch(level) {
    case level::trace:
        spdlog_level = spdlog::level::trace;
        sdl_log_level = SDL_LOG_PRIORITY_VERBOSE;
        break;
    case level::debug:
        spdlog_level = spdlog::level::debug;
        sdl_log_level = SDL_LOG_PRIORITY_DEBUG;
        break;
    case level::info:
        spdlog_level = spdlog::level::info;
        sdl_log_level = SDL_LOG_PRIORITY_INFO;
        break;
    case level::warning:
        spdlog_level = spdlog::level::warn;
        sdl_log_level = SDL_LOG_PRIORITY_WARN;
        break;
    [[likely]] case level::error:
        spdlog_level = spdlog::level::err;
        sdl_log_level = SDL_LOG_PRIORITY_ERROR;
        break;
    case level::critical:
        spdlog_level = spdlog::level::critical;
        sdl_log_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    case level::off:
        spdlog_level = spdlog::level::off;
        sdl_log_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    [[unlikely]] default:
        spdlog_level = spdlog::level::info;
        sdl_log_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    }
    spdlog::set_level(spdlog_level);
    SDL_LogSetAllPriority(sdl_log_level);
}

auto level_from_string(const std::string &log_level) -> level::log_level {
    using namespace entt::literals; // NOLINT(google-build-using-namespace)

    switch(entt::hashed_string{log_level.c_str()}) {
    case "trace"_hs:
        return logger::level::trace;
    case "debug"_hs:
        return logger::level::debug;
    case "info"_hs:
        return logger::level::info;
    case "warning"_hs:
        return logger::level::warning;
    [[likely]] case "error"_hs:
        return logger::level::error;
    case "critical"_hs:
        return logger::level::critical;
    case "off"_hs:
        return logger::level::off;
    [[unlikely]] default:
        return logger::level::error;
    }
}

// NOLINTBEGIN(bugprone-branch-clone)
auto string_from_level(level::log_level level) -> std::string {
    using namespace std::string_literals; // NOLINT(google-build-using-namespace)

    switch(level) {
    case level::trace:
        return "trace"s;
    case level::debug:
        return "debug"s;
    case level::info:
        return "info"s;
    case level::warning:
        return "warning"s;
    [[likely]] case level::error:
        return "error"s;
    case level::critical:
        return "critical"s;
    [[unlikely]] default:
        return "off"s;
    }
}
// NOLINTEND(bugprone-branch-clone)

} // namespace sneze::logger
