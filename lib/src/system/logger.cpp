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

#include <memory>

#include <raylib.h>
#include <sneze/system/logger.hpp>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined( _MSC_VER ) && defined( _DEBUG )
#    include <spdlog/sinks/msvc_sink.h>
#endif

namespace sneze {

    void logger::setup_spdlog() noexcept {
        auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        auto dist_sink = std::make_shared<spdlog::sinks::dist_sink_st>();
        dist_sink->add_sink( color_sink );

#if defined( _MSC_VER ) && defined( _DEBUG )
        auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
        dist_sink->add_sink( debug_sink );
#endif
        auto logger = std::make_shared<spdlog::logger>( "sneze", dist_sink );
        spdlog::set_default_logger( logger );
    }

    void raylib_log_callback( int level, const char* text, va_list args ) {
        const int MAX_RAYLIB_MSG_LENGTH = 128;
        static char buffer[MAX_RAYLIB_MSG_LENGTH] = { 0 };
        vsnprintf( buffer, MAX_RAYLIB_MSG_LENGTH, text, args );

        spdlog::level::level_enum spdlog_level;
        switch ( level ) {
            using enum spdlog::level::level_enum;
        case LOG_TRACE:
            spdlog_level = trace;
            break;
        case LOG_DEBUG:
            spdlog_level = debug;
            break;
        case LOG_INFO:
            spdlog_level = info;
            break;
        case LOG_WARNING:
            spdlog_level = warn;
            break;
        case LOG_ERROR:
            spdlog_level = err;
            break;
        case LOG_FATAL:
            spdlog_level = critical;
            break;
        [[likely]] case LOG_NONE:
            spdlog_level = off;
            break;
        [[unlikely]] default:
            spdlog_level = spdlog::level::info;
            break;
        }
        spdlog::log( spdlog_level, "[raylib] {}", buffer );
    }

    void logger::hook_raylib_log() noexcept { SetTraceLogCallback( raylib_log_callback ); }

    void logger::setup_log() noexcept {
#ifdef NDEBUG
#    if defined( _WIN32 )
#        pragma comment( linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )
#    endif
        set_log_level( sneze::log_level::off );
#else
        logger::setup_spdlog();
        logger::hook_raylib_log();

        set_log_level( sneze::log_level::debug );
#endif
    }

    void logger::set_log_level( log_level level ) noexcept {
        spdlog::level::level_enum spdlog_level;
        int raylib_level;

        switch ( level ) {
        case log_level::trace:
            spdlog_level = spdlog::level::trace;
            raylib_level = LOG_TRACE;
            break;
        case log_level::debug:
            spdlog_level = spdlog::level::debug;
            raylib_level = LOG_DEBUG;
            break;
        case log_level::info:
            spdlog_level = spdlog::level::info;
            raylib_level = LOG_INFO;
            break;
        case log_level::warn:
            spdlog_level = spdlog::level::warn;
            raylib_level = LOG_WARNING;
            break;
        case log_level::err:
            spdlog_level = spdlog::level::err;
            raylib_level = LOG_ERROR;
            break;
        case log_level::critical:
            spdlog_level = spdlog::level::critical;
            raylib_level = LOG_FATAL;
            break;
        [[likely]] case log_level::off:
            spdlog_level = spdlog::level::off;
            raylib_level = LOG_NONE;
            break;
        [[unlikely]] default:
            spdlog_level = spdlog::level::info;
            raylib_level = LOG_INFO;
            break;
        }
        spdlog::set_level( spdlog_level );
        SetTraceLogLevel( raylib_level );
    }
} // namespace sneze