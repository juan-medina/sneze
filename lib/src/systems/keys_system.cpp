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

#include "sneze/systems/keys_system.hpp"

#include "sneze/app/world.hpp"
#include "sneze/platform/logger.hpp"

namespace sneze {

void keys_system::init(world *world) {
    logger::trace("init key system");
    if(exit_.key != keyboard::key::unknown) {
        logger::trace("exit full screen key: [{}]", exit_.string());
    }
    if(toggle_full_screen_.key != keyboard::key::unknown) {
        logger::trace("toggle full screen key: [{}]", toggle_full_screen_.string());
    }
    world->add_listener<events::key_up, &keys_system::key_up>(this);
}

void keys_system::end(world *world) {
    logger::trace("end key system");
    world->remove_listeners(this);
}

void keys_system::key_up(const events::key_up &event) {
    if(event == exit_) {
        event.world->emmit<events::application_want_closing>();
    } else if(event == toggle_full_screen_) {
        event.world->emmit<events::toggle_fullscreen>();
    }
}

void keys_system::update(world * /*world*/) {}

} // namespace sneze