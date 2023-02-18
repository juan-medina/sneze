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
#include "sneze/platform/logger.hpp"

#include <raylib.h>

namespace sneze {
void render_system::update(world &world) {
    world.sort<components::renderable>(render_system::sort_by_depth);

    render_->begin_frame();

    using renderable = components::renderable;
    using position = components::position;
    using label = components::label;

    for(auto const &&[id, renderable, position, color]: world.view<const renderable, const position, const color>()) {
        if(renderable.visible) {
            if(auto lbl = world.has<label>(id)) {
                render_->draw_label(*lbl, position, color);
            }
        }
    }

    render_->end_frame();

    if(WindowShouldClose()) world.emmit(events::application_want_closing{});
}

void render_system::init(world &) {
    logger::debug("render_system::init");
}

void render_system::end(world &) {
    logger::debug("render_system::end");
}

} // namespace sneze
