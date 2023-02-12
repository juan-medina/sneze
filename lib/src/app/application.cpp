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

#include <sneze/app/application.hpp>
#include <sneze/platform/version.hpp>
#include <sneze/system/logger.hpp>

#include <boxer/boxer.h>
#include <fmt/format.h>

namespace sneze {

    result<bool, error> application::run() {
        logger::setup_log();

        logger::info( "{}", VERSION );
        logger::debug( "Running application: {} (Team: {})", name(), team() );

        if ( auto err = read_settings().ko() ) return show_error( *err );
        if ( auto err = launch().ko() ) return show_error( *err );
        if ( auto err = save_settings().ko() ) return show_error( *err );

        logger::debug( "Stopping application: {}", name() );

        return true;
    }

    result<> application::launch() noexcept {
        auto width = settings_.get( "window", "width", 1920LL );
        auto height = settings_.get( "window", "height", 1080LL );

        auto setup = init();

        logger::debug( "Init render" );

        if ( auto err = render_.init( width, height, name(), setup.clear_color_ ).ko() ) {
            logger::error( "error initializing render" );
            return error( "Can't init the render system.", *err );
        }

        logger::debug( "Render created" );

        while ( true ) {
            if ( auto [val, err] = render_.want_to_close().ok(); err || *val ) { break; }

            render_.begin_frame();

            // raylib::DrawText( "Congrats! You created your first window!", 190, 200, 20, raylib::LIGHTGRAY );

            render_.end_frame();
        }

        logger::debug( "Ending render" );

        render_.end();

        logger::debug( "Render ended" );

        end();

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

    result<> application::read_settings() noexcept {
        if ( auto [val, err] = settings_.read().ok(); err ) {
            logger::error( "error reading settings" );
            return error( "Can't read settings.", *err );
        } else {
            return *val;
        }
    }

    result<> application::save_settings() noexcept {
        if ( auto [val, err] = settings_.save().ok(); err ) {
            logger::error( "error saving settings" );
            return error( "Can't save settings.", *err );
        } else {
            return *val;
        }
    }

} // namespace sneze
