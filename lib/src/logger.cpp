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

#include "logger.h"
#include "raylib.h"

namespace sneze {

void raylib_log_callback(int level, const char *text, va_list args) {
    const int MAX_RAYLIB_MSG_LENGTH = 128;
    static char buffer[MAX_RAYLIB_MSG_LENGTH] = {0};
    vsprintf_s(buffer, MAX_RAYLIB_MSG_LENGTH, text, args);

    spdlog::level::level_enum spdlog_level;
    switch(level) {
    case LOG_TRACE:
        spdlog_level = spdlog::level::trace;
        break;
    case LOG_DEBUG:
        spdlog_level = spdlog::level::debug;
        break;
    case LOG_INFO:
        spdlog_level = spdlog::level::info;
        break;
    case LOG_WARNING:
        spdlog_level = spdlog::level::warn;
        break;
    case LOG_ERROR:
        spdlog_level = spdlog::level::err;
        break;
    case LOG_FATAL:
        spdlog_level = spdlog::level::critical;
        break;
    case LOG_NONE:
        spdlog_level = spdlog::level::off;
        break;
    default:
        spdlog_level = spdlog::level::info;
        break;
    }
    spdlog::log(spdlog_level, "[raylib] {}", buffer);
}

void hook_raylib_log() noexcept {
    SetTraceLogCallback(raylib_log_callback);
}

void set_log_level(log_level level) noexcept {
    spdlog::level::level_enum spdlog_level;
    int raylib_level;

    switch(level) {
    case trace:
        spdlog_level = spdlog::level::trace;
        raylib_level = LOG_TRACE;
        break;
    case debug:
        spdlog_level = spdlog::level::debug;
        raylib_level = LOG_DEBUG;
        break;
    case info:
        spdlog_level = spdlog::level::info;
        raylib_level = LOG_INFO;
        break;
    case warn:
        spdlog_level = spdlog::level::warn;
        raylib_level = LOG_WARNING;
        break;
    case err:
        spdlog_level = spdlog::level::err;
        raylib_level = LOG_ERROR;
        break;
    case critical:
        spdlog_level = spdlog::level::critical;
        raylib_level = LOG_FATAL;
        break;
    case off:
        spdlog_level = spdlog::level::off;
        raylib_level = LOG_NONE;
        break;
    default:
        spdlog_level = spdlog::level::info;
        raylib_level = LOG_INFO;
        break;
    }
    spdlog::set_level(spdlog_level);
    SetTraceLogLevel(raylib_level);
}

} // namespace sneze
