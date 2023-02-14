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

#include "sneze/systems/render_system.hpp"

#include "sneze/events/events.hpp"

#include "raylib.h"

namespace sneze {
    void render_system::update( sneze::world& world ) {
        render_->begin_frame();

        using namespace components;
        world.sort<renderable>( render_system::sort_by_depth );

        for ( auto&& [id, renderable, position, color] : world.view<const renderable, const position, const color>() ) {
            if ( renderable.visible_ ) {
                if ( auto txt = world.has<text>( id ) ) { render_->draw_text( *txt, position, color ); }
            }
        }

        render_->end_frame();

        if ( WindowShouldClose() ) world.emmit( events::application_want_closing{} );
    }
} // namespace sneze