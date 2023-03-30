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

#include "counter_system.hpp"

#include <fmt/format.h>

// init the counter system
void counter_system::init(sneze::world * /*world*/) {
    sneze::logger::debug("counter_system::init");
}

// shutdown the counter system
void counter_system::end(sneze::world * /*world*/) {
    sneze::logger::debug("counter_system::end");
}

// update the counter system
void counter_system::update(sneze::world *world) {
    // get the global acceleration
    const auto acc = world->get_global<acceleration>();
    // get the global game time
    const auto time = world->get_global<sneze::game_time>();

    // get all the entities with counter and label components
    for(auto &&[entity, counter, label]: world->get_entities<counter, sneze::components::label>()) {
        // update the counter
        counter.value -= static_cast<int>(static_cast<float>(acc.value) * time.delta);
        // if the counter is less than 0, remove the entity
        if(counter.value < 0) {
            world->remove_entity(entity);
            continue;
        }
        // update the label
        label.text = fmt::format("Counter: {}, elapsed: {}", counter.value, time.elapsed);
    }
}
