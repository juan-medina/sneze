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
#include "sneze/platform/logger.hpp"

namespace sneze {

void layout_system::init(sneze::world *world) {
    logger::debug("init layout system");
    world->add_listener<events::window_resized, &layout_system::window_resized>(this);
}

void layout_system::end(sneze::world *world) {
    logger::debug("end layout system");
    world->remove_listeners(this);
}

void layout_system::update(sneze::world *) {}

void layout_system::window_resized(const events::window_resized &event) {
    logger::debug("window resized: [{}x{}]", event.width, event.height);
}

} // namespace sneze