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

#include "sneze/platform/logger.hpp"
#include "sneze/render/render.hpp"

namespace sneze {

render_system::render_system(std::shared_ptr<sneze::render> render): render_{std::move(render)} {}

void render_system::init(world *world) {
    logger::debug("init render system");
    world->add_listener<events::toggle_fullscreen, &render_system::toggle_fullscreen>(this);
}

void render_system::end(world *world) {
    logger::debug("end render system");
    world->remove_listeners(this);
}

void render_system::update(world *world) {
    world->sort<components::renderable>(render_system::sort_by_depth);

    render_->begin_frame();

    using color = components::color;
    using renderable = components::renderable;
    using position = components::position;
    using label = components::label;
    using layout = components::layout;

    for(auto const [id, renderable, color, pos]: world->entities<const renderable, const color, const position>()) {
        if(renderable.visible) {
            // local position
            auto draw_position = pos;

            // layout position
            if(auto lay = world->has_component<layout>(id)) {
                draw_position = *lay;
            }

            if(auto lbl = world->has_component<label>(id)) {
                render_->draw_label(*lbl, draw_position, color);
            } else if(auto line = world->has_component<components::line>(id)) {
                render_->draw_line(*line, draw_position, color);
            } else if(auto box = world->has_component<components::box>(id)) {
                render_->draw_box(*box, draw_position, color);
            }
        }
    }

    render_->end_frame();
}

void render_system::toggle_fullscreen(const events::toggle_fullscreen &) noexcept {
    render_->toggle_fullscreen();
}

} // namespace sneze
