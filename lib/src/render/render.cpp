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

#include <sneze/render/render.hpp>
#include <sneze/system/logger.hpp>

namespace raylib {
#include <raylib.h>
}

namespace sneze {
    result<> render::init( const std::int64_t& width,
                           const std::int64_t& height,
                           const std::string& title,
                           const color& color ) {
        logger::debug( "Creating window" );
        raylib::InitWindow( (int)width, (int)height, title.c_str() );
        clear_color( color );
        return true;
    }

    void render::end() {
        logger::debug( "Closing window" );
        raylib::CloseWindow();
    }

    void render::begin_frame() {
        raylib::BeginDrawing();

        ClearBackground( clear_color_.to_raylib() );
    }

    void render::end_frame() { raylib::EndDrawing(); }

    result<> render::want_to_close() const { return raylib::WindowShouldClose(); }

} // namespace sneze