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
    logger::debug("init key system");
    if(exit_key_modifier_.key != keyboard::key::unknown) {
        logger::debug("exit key: {}, mod: {}", exit_key_modifier_.key, exit_key_modifier_.modifier);
    } else {
        logger::debug("no exit key");
    }
    if(toggle_full_screen_key_modifier_.key != keyboard::key::unknown) {
        logger::debug("toggle full screen key: {}, mod: {}",
                      toggle_full_screen_key_modifier_.key,
                      toggle_full_screen_key_modifier_.modifier);
    } else {
        logger::debug("no toggle full screen key");
    }
    world->add_listener<events::key_up, &keys_system::key_up>(this);
}

void keys_system::end(world *world) {
    logger::debug("end key system");
    world->remove_listeners(this);
}

void keys_system::key_up(const events::key_up &event) {
    if(event == exit_key_modifier_) {
        event.world->emmit<events::application_want_closing>();
    } else if(event == toggle_full_screen_key_modifier_) {
        event.world->emmit<events::toggle_fullscreen>();
    }
}

void keys_system::update(world *) {}

} // namespace sneze