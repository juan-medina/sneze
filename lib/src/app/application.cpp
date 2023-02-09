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

#include <boxer/boxer.h>
#include <fmt/format.h>
#include <raylib.h>
#include <sneze/app/application.hpp>
#include <sneze/platform/version.hpp>
#include <sneze/system/logger.hpp>

namespace sneze {

    result<bool, error> application::run() {
        logger::setup_log();

        logger::info( "{}", VERSION );
        logger::debug( "Running application: {} (Team: {})", name(), team() );

        if ( auto err = read_config().ko() ) return show_error( *err );
        if ( auto err = launch().ko() ) return show_error( *err );
        if ( auto err = save_config().ko() ) return show_error( *err );

        logger::debug( "Stopping application: {}", name() );

        return true;
    }

    result<> application::launch() noexcept {
        logger::debug( "Triggering On Start" );
        on_start();

        logger::debug( "Creating Window" );

        auto width = config_.get_value( "window", "width", 1920LL );
        auto height = config_.get_value( "window", "height", 1080LL );

        InitWindow( (int)width, (int)height, name().c_str() );

        logger::debug( "Window Created" );

        while ( !WindowShouldClose() ) // Detect window close button or ESC key
        {
            BeginDrawing();

            ClearBackground( RAYWHITE );

            DrawText( "Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY );

            EndDrawing();
        }

        logger::debug( "Closing Window" );

        CloseWindow();

        logger::debug( "Window Closed" );

        logger::debug( "Triggering On End" );
        on_end();

        return true;
    }

    const error& application::show_error( const error& err ) const noexcept {
        std::string message = err.message();
        auto causes = err.causes();
        if ( !causes.empty() ) {
            message += " Caused by:\n";
            for ( const auto& cause : causes ) {
                message += "\n - " + cause;
            }
        }
        auto message_title = fmt::format( "{} : Error!", name() );
        boxer::show( message.c_str(), message_title.c_str(), boxer::Style::Error, boxer::Buttons::Quit );
        return err;
    }

    result<> application::read_config() noexcept {
        if ( auto [val, err] = config_.read().ok(); err ) {
            logger::error( "error reading config" );
            return error( "Can't read config.", *err );
        } else {
            return *val;
        }
    }

    result<> application::save_config() noexcept {
        if ( auto [val, err] = config_.save().ok(); err ) {
            logger::error( "error saving config" );
            return error( "Can't save config.", *err );
        } else {
            return *val;
        }
    }

} // namespace sneze
