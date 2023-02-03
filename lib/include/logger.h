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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include "spdlog/spdlog.h"

namespace sneze {

enum log_level : int {
    trace = spdlog::level::trace,
    debug = spdlog::level::debug,
    info = spdlog::level::info,
    warn = spdlog::level::warn,
    err = spdlog::level::err,
    critical = spdlog::level::critical,
    off = spdlog::level::off
};

#define LOG_TRACE(s, ...) \
    spdlog::trace("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

#define LOG_DEBUG(s, ...) \
    spdlog::debug("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

#define LOG_INFO(s, ...) \
    spdlog::info("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

#define LOG_WARN(s, ...) \
    spdlog::warn("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

#define LOG_ERR(s, ...) \
    spdlog::error("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

#define LOG_CRITICAL(s, ...) \
    spdlog::critical("[{}] " #s " -> {} ({})", __FUNCTION__, ##__VA_ARGS__, __FILE__, __LINE__)

void hook_raylib_log() noexcept;

void set_log_level(log_level level) noexcept;

} // namespace sneze

#pragma clang diagnostic pop