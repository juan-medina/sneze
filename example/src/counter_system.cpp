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

namespace logger = sneze::logger;
namespace components = sneze::components;

void counter_system::init(sneze::world *) {
    logger::debug("counter_system::init");
}

void counter_system::end(sneze::world *) {
    logger::debug("counter_system::end");
}

void counter_system::update(sneze::world *world) {
    const auto acc = world->global<acceleration>();

    for(auto &&[_, ct, lbl]: world->view<counter, components::label>()) {
        ct.value += static_cast<int>(acc.value * world->delta());
        lbl.text = fmt::format("Counter: {}", ct.value);
    }
}
