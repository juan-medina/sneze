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

#include <spdlog/spdlog.h>

#if __CLION_IDE__
#    pragma clang diagnostic push
#    pragma ide diagnostic ignored "OCUnusedMacroInspection"
#endif

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

    class logger {
    public:
        template <typename... Args>
        struct info {
            explicit info( fmt::format_string<Args...> fmt,
                           Args&&... args,
                           const std::source_location& location = std::source_location::current() ) {
                log( log_level::info, location, fmt, std::forward<Args>( args )... );
            }
        };

        template <typename... Args>
        info( fmt::format_string<Args...> fmt, Args&&... ) -> info<Args...>;

        template <typename... Args>
        struct debug {
            explicit debug( fmt::format_string<Args...> fmt,
                            Args&&... args,
                            const std::source_location& location = std::source_location::current() ) {
                log( log_level::debug, location, fmt, std::forward<Args>( args )... );
            }
        };

        template <typename... Args>
        debug( fmt::format_string<Args...> fmt, Args&&... ) -> debug<Args...>;

        template <typename... Args>
        struct error {
            explicit error( fmt::format_string<Args...> fmt,
                            Args&&... args,
                            const std::source_location& location = std::source_location::current() ) {
                log( log_level::err, location, fmt, std::forward<Args>( args )... );
            }
        };

        template <typename... Args>
        error( fmt::format_string<Args...> fmt, Args&&... ) -> error<Args...>;

        template <typename... Args>
        struct warning {
            explicit warning( fmt::format_string<Args...> fmt,
                              Args&&... args,
                              const std::source_location& location = std::source_location::current() ) {
                log( log_level::warn, location, fmt, std::forward<Args>( args )... );
            }
        };

        template <typename... Args>
        warning( fmt::format_string<Args...> fmt, Args&&... ) -> warning<Args...>;

        static void setup_log() noexcept;
        static void set_log_level( log_level level ) noexcept;

    private:
        explicit logger(log_level level) {
            setup_log();
        }
        virtual ~logger() = default;

    private:
        static logger logger_;
#if defined( NDEBUG )
        template <typename... Args>
        static void log( Args&&... ) {}
#else
        template <typename... Args>
        static void
        log( const log_level level, std::source_location source, fmt::format_string<Args...> fmt, Args&&... args ) {

            using namespace spdlog;
            auto location = source_loc{ source.file_name(), static_cast<int>( source.line() ), source.function_name() };
            spdlog::log( location, static_cast<level::level_enum>( level ), fmt, std::forward<Args>( args )... );
        }
#endif

        static void hook_raylib_log() noexcept;
        static void setup_spdlog() noexcept;
    };

} // namespace sneze

#if __CLION_IDE__
#    pragma clang diagnostic pop
#endif