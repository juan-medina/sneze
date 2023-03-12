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

#include "sneze/effects/effects_system.hpp"

#include "sneze/app/world.hpp"
#include "sneze/effects/effects.hpp"
#include "sneze/globals/globals.hpp"
#include "sneze/platform/logger.hpp"

namespace sneze {

void effects_system::init(world * /*world*/) {
    logger::trace("effects system::init");
}

void effects_system::end(world * /*world*/) {
    logger::trace("effects system::end");
}

void effects_system::update(world *world) {
    auto time = world->get_global<game_time>();
    for(auto const &&[entity, alternate_color, color]: world->entities<effects::alternate_color, components::color>()) {
        color = alternate_color.from;
        alternate_color.current_time += time.delta;
        if(alternate_color.pause) {
            if(alternate_color.current_time > alternate_color.delay) {
                alternate_color.pause = false;
                alternate_color.current_time = 0.0F;
            }
            continue;
        }
        if(alternate_color.current_time > alternate_color.time) {
            alternate_color.current_time = 0.0F;
            alternate_color.pause = true;
            std::swap(alternate_color.from, alternate_color.to);
            continue;
        }
        color.blend(alternate_color.to, alternate_color.current_time / alternate_color.time);
    }
}

} // namespace sneze