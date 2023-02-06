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
#include <raylib.h>
#include <sneze/application.h>
#include <sneze/logger.h>

int screenWidth = 800;
int screenHeight = 450;

namespace sneze {

    result<bool, error> application::run() {
        if ( auto [val, err] = launch().check(); err ) {
            std::string message = err->message();
            auto causes = err->causes();
            if ( !causes.empty() ) {
                message += " Caused by:\n";
                for ( const auto& cause : causes ) {
                    message += "\n - " + cause;
                }
            }
            auto message_title = std::format( "{} : Error!", name() );
            boxer::show( message.c_str(), message_title.c_str(), boxer::Style::Error, boxer::Buttons::Quit );
            return *err;
        } else {
            return *val;
        }
    }
    result<bool, error> application::launch() {
        setup_log();

        LOG_DEBUG( "Starting application: {} (Team: {})", name(), team() );

        if ( auto [val, err] = config_.read().check(); err ) {
            LOG_ERR( "error reading config" );
            return error( "Can't read config.", *err );
        }

        LOG_DEBUG( "Triggering On Start" );
        on_start();

        LOG_DEBUG( "Creating Window" );

        InitWindow( screenWidth, screenHeight, name().c_str() );

        LOG_DEBUG( "Window Created" );

        while ( !WindowShouldClose() ) // Detect window close button or ESC key
        {
            BeginDrawing();

            ClearBackground( RAYWHITE );

            DrawText( "Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY );

            EndDrawing();
        }

        LOG_DEBUG( "Closing Window" );

        CloseWindow();

        LOG_DEBUG( "Window Closed" );

        LOG_DEBUG( "Triggering On End" );
        on_end();

        LOG_DEBUG( "Stopping application: {}", name() );

        return true;
    }

} // namespace sneze
