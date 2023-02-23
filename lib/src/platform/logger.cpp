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

#include <entt/entt.hpp>
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
    auto logger = std::make_shared<spdlog::logger>("sneze", dist_sink);
    spdlog::set_default_logger(logger);
}

void setup_log() {
    setup_spdlog();
#ifdef NDEBUG
    set_level(logger::level::info);
#else
    set_level(logger::level::debug);
#endif
}

void set_level(level::log_level level) {
    spdlog::level::level_enum spdlog_level; // NOLINT(cppcoreguidelines-init-variables)

    switch(level) {
    case level::trace:
        spdlog_level = spdlog::level::trace;
        break;
    case level::debug:
        spdlog_level = spdlog::level::debug;
        break;
    case level::info:
        spdlog_level = spdlog::level::info;
        break;
    case level::warning:
        spdlog_level = spdlog::level::warn;
        break;
    case level::error:
        spdlog_level = spdlog::level::err;
        break;
    case level::critical:
        spdlog_level = spdlog::level::critical;
        break;
    [[likely]] case level::off:
        spdlog_level = spdlog::level::off;
        break;
    [[unlikely]] default:
        spdlog_level = spdlog::level::info;
        break;
    }
    spdlog::set_level(spdlog_level);
}

auto level_from_string(const std::string &log_level) -> level::log_level {
    using namespace entt::literals; // NOLINT(google-build-using-namespace)

    switch(entt::hashed_string{log_level.c_str()}) {
    case "trace"_hs:
        return logger::level::trace;
        break;
    case "debug"_hs:
        return logger::level::debug;
        break;
    case "info"_hs:
        return logger::level::info;
        break;
    case "warning"_hs:
        return logger::level::warning;
        break;
    [[likely]] case "error"_hs:
        return logger::level::error;
        break;
    case "critical"_hs:
        return logger::level::critical;
        break;
    case "off"_hs:
        return logger::level::off;
        break;
    [[unlikely]] default:
        return logger::level::error;
        break;
    }
}

auto string_from_level(level::log_level level) -> std::string {
    using namespace std::string_literals; // NOLINT(google-build-using-namespace)

    switch(level) {
    case level::trace:
        return "trace"s;
        break;
    case level::debug:
        return "debug"s;
        break;
    case level::info:
        return "info"s;
        break;
    case level::warning:
        return "warning"s;
        break;
    case level::error:
        return "error"s;
        break;
    case level::critical:
        return "critical"s;
        break;
    [[likely]] case level::off:
        return "off"s;
        break;
    [[unlikely]] default:
        return "info"s;
        break;
    }
}

} // namespace sneze::logger
