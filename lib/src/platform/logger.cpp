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

#include "sneze/platform/version.hpp"

#include <memory>

#include <entt/core/hashed_string.hpp>
#include <SDL_log.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#    include <spdlog/sinks/msvc_sink.h>
#endif

namespace sneze::logger {

template<uint8_t R, uint8_t G, uint8_t B>
auto color_rgb() {
    return fmt::format("\033[38;2;{};{};{}m", R, G, B);
}

auto color_reset() {
    return "\033[0m";
}

class color_tag_start: public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg &msg, const std::tm & /*time*/, spdlog::memory_buf_t &dest) override {
        std::string color_code;

        switch(msg.level) {
        case spdlog::level::trace:
            color_code = color_rgb<0x80, 0x80, 0x80>();
            break;
        case spdlog::level::info:
            color_code = color_rgb<0x7E, 0xFF, 0x00>();
            break;
        case spdlog::level::warn:
            color_code = color_rgb<0xFF, 0x8C, 0x00>();
            break;
        [[likely]] case spdlog::level::err:
            color_code = color_rgb<0xFF, 0x00, 0x0E>();
            break;
        [[unlikely]] default:
            break;
        }

        if(!color_code.empty()) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            dest.append(color_code.data(), color_code.data() + color_code.size());
        }
    }

    [[nodiscard]] auto clone() const -> std::unique_ptr<custom_flag_formatter> override {
        return spdlog::details::make_unique<color_tag_start>();
    }
    static constexpr auto token = '[';
};

class color_tag_end: public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg &msg, const std::tm & /*time*/, spdlog::memory_buf_t &dest) override {
        std::string color_code;

        switch(msg.level) {
        case spdlog::level::trace:
        case spdlog::level::info:
        case spdlog::level::warn:
        [[likely]] case spdlog::level::err:
            color_code = color_reset();
            break;
        [[unlikely]] default:
            break;
        }

        if(!color_code.empty()) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            dest.append(color_code.data(), color_code.data() + color_code.size());
        }
    }

    [[nodiscard]] auto clone() const -> std::unique_ptr<custom_flag_formatter> override {
        return spdlog::details::make_unique<color_tag_end>();
    }

    static constexpr auto token = ']';
};

void setup_spdlog() {
    using namespace std::string_literals;
    static const auto initial_pattern = "%v"s;
    auto default_pattern = "[%x] [%H:%M:%S.%e] [%^%-8l%$] %v"s;
#if not defined(NDEBUG) && not defined(NO_SOURCE_LOCATION)
    default_pattern += " -> %@"s;
#endif
    auto dist_sink = std::make_shared<spdlog::sinks::dist_sink_st>();

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_st>();
    stdout_sink->set_pattern(color_rgb<0x1E, 0x80, 0xFF>() + initial_pattern + color_reset());
    dist_sink->add_sink(stdout_sink);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("sneze.log", true);
    file_sink->set_pattern(initial_pattern);
    dist_sink->add_sink(file_sink);

#if defined(_MSC_VER) && defined(_DEBUG)
    auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
    debug_sink->set_pattern(initial_pattern);
    dist_sink->add_sink(debug_sink);
#endif

    auto logger = std::make_shared<spdlog::logger>("sneze", dist_sink);
    spdlog::set_default_logger(logger);
    spdlog::info(version::logo_string);

    auto color_pattern = "%"s + color_tag_start::token + default_pattern + "%"s + color_tag_end::token;
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<color_tag_start>(color_tag_start::token)
        .add_flag<color_tag_end>(color_tag_end::token)
        .set_pattern(color_pattern);
    stdout_sink->set_formatter(std::move(formatter));

    file_sink->set_pattern(default_pattern);

#if defined(_MSC_VER) && defined(_DEBUG)
    debug_sink->set_pattern(default_pattern);
#endif
}

void sdl_log_callback(void * /*user_data*/, int /*category*/, SDL_LogPriority priority, const char *message) {
    level level; // NOLINT(cppcoreguidelines-init-variables)

    switch(priority) {
    case SDL_LOG_PRIORITY_VERBOSE:
        level = level::trace;
        break;
    case SDL_LOG_PRIORITY_DEBUG:
        level = level::debug;
        break;
    case SDL_LOG_PRIORITY_INFO:
        level = level::info;
        break;
    case SDL_LOG_PRIORITY_WARN:
        level = level::warning;
        break;
    [[likely]] case SDL_LOG_PRIORITY_ERROR:
        level = level::error;
        break;
    case SDL_LOG_PRIORITY_CRITICAL:
        level = level::critical;
        break;
    [[unlikely]] default:
        level = level::off;
        break;
    }
    log(level, std::source_location::current(), "SDL: {}", message);
}

void setup_log() {
    setup_spdlog();
    SDL_LogSetOutputFunction(sdl_log_callback, nullptr);
#ifdef NDEBUG
    set_level(level::info);
#else
    set_level(level::trace);
#endif
}

void set_level(level level) {
    spdlog::level::level_enum spdlevel; // NOLINT(cppcoreguidelines-init-variables)
    SDL_LogPriority sdl_level;          // NOLINT(cppcoreguidelines-init-variables)

    switch(level) {
    case level::trace:
        spdlevel = spdlog::level::trace;
        sdl_level = SDL_LOG_PRIORITY_VERBOSE;
        break;
    case level::debug:
        spdlevel = spdlog::level::debug;
        sdl_level = SDL_LOG_PRIORITY_DEBUG;
        break;
    case level::info:
        spdlevel = spdlog::level::info;
        sdl_level = SDL_LOG_PRIORITY_INFO;
        break;
    case level::warning:
        spdlevel = spdlog::level::warn;
        sdl_level = SDL_LOG_PRIORITY_WARN;
        break;
    [[likely]] case level::error:
        spdlevel = spdlog::level::err;
        sdl_level = SDL_LOG_PRIORITY_ERROR;
        break;
    case level::critical:
        spdlevel = spdlog::level::critical;
        sdl_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    case level::off:
        spdlevel = spdlog::level::off;
        sdl_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    [[unlikely]] default:
        spdlevel = spdlog::level::info;
        sdl_level = SDL_LOG_PRIORITY_CRITICAL;
        break;
    }
    spdlog::set_level(spdlevel);
    SDL_LogSetAllPriority(sdl_level);
}

auto level_from_string(const std::string &level_string) -> level {
    using namespace entt::literals; // NOLINT(google-build-using-namespace)

    switch(entt::hashed_string{level_string.c_str()}) {
    case "trace"_hs:
        return level::trace;
    case "debug"_hs:
        return level::debug;
    case "info"_hs:
        return level::info;
    case "warning"_hs:
        return level::warning;
    [[likely]] case "error"_hs:
        return level::error;
    case "critical"_hs:
        return level::critical;
    case "off"_hs:
        return level::off;
    [[unlikely]] default:
        return level::error;
    }
}

// NOLINTBEGIN(bugprone-branch-clone)
auto string_from_level(level level) -> std::string {
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
