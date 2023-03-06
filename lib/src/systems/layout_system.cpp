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

#include "sneze/systems/layout_system.hpp"

#include "sneze/app/world.hpp"
#include "sneze/components/ui.hpp"
#include "sneze/platform/logger.hpp"

namespace sneze {

void layout_system::init(sneze::world *world) {
    logger::debug("init layout system");
    world->add_listener<events::window_resized, &layout_system::window_resized>(this);

    world->add_listener_to_add_component<components::anchor, &layout_system::add_component_anchor>(this);
}

void layout_system::end(sneze::world *world) {
    logger::debug("end layout system");
    world->remove_listeners(this);

    world->remove_component_listeners<components::anchor>(this);
}

void layout_system::update(sneze::world *) {}

void layout_system::window_resized(const events::window_resized &event) {
    logical_ = event.logical;
    using anchor = components::anchor;

    for(auto const &&[entity, anc]: event.world->entities<anchor>()) {
        calculate_layout(event.world, entity, anc);
    }
}

void layout_system::add_component_anchor(events::add_component<components::anchor> const &event) {
    calculate_layout(event.world, event.entity, event.component);
}

void layout_system::calculate_layout(world *world, const entt::entity entity, const components::anchor &anc) {
    using layout = components::layout;
    using position = components::position;
    using horizontal = components::horizontal;
    using vertical = components::vertical;

    auto lay = layout{};

    switch(anc.horizontal) {
    case horizontal::none:
    case horizontal::left:
        lay.x = logical_.position.x;
        break;
    case horizontal::center:
        lay.x = logical_.position.x + (logical_.size.width / 2);
        break;
    case horizontal::right:
        lay.x = logical_.position.x + logical_.size.width;
        break;
    }

    switch(anc.vertical) {
    case vertical::none:
    case vertical::top:
        lay.y = logical_.position.y;
        break;
    case vertical::center:
        lay.y = logical_.position.y + (logical_.size.height / 2);
        break;
    case vertical::bottom:
        lay.y = logical_.position.y + logical_.size.height;
        break;
    }

    if(auto pos = world->has_component<position>(entity)) {
        lay.x += pos->x;
        lay.y += pos->y;
    } else {
        world->set_component<position>(entity, position{0, 0});
    }

    if(auto current = world->has_component<layout>(entity)) {
        *current = lay;
    } else {
        world->set_component<layout>(entity, lay);
    }
}

} // namespace sneze