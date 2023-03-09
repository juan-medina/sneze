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

#include "draw_game.hpp"

draw_game::draw_game(): application("sneze", "Draw Game") {}

namespace logger = sneze::logger;
using config = sneze::config;
namespace components = sneze::components;
using color = components::color;

auto draw_game::configure() -> config {
    logger::debug("configure");

    namespace keyboard = sneze::keyboard;
    using key = keyboard::key;
    using modifier = keyboard::modifier;

    return config()
        .size(1920, 1080)
        .clear(color::light_gray)
        .exit(key::escape)
        .toggle_full_screen(modifier::alt, key::_return);
}

auto draw_game::init() -> sneze::result<> {
    logger::debug("init draw game");

    using rendereable = components::renderable;
    using line = components::line;
    using box = components::box;
    using position = components::position;

    const auto line_thickness = 15.f;
    const auto line_length = 400;
    auto start_position = position{line_length, (1080.f / 2) - (line_length / 2.f)};

    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y},
                        line{{start_position.x + line_length, start_position.y}, line_thickness},
                        color::red);
    world()->add_entity(rendereable{},
                        position{start_position.x + line_length, start_position.y},
                        line{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
                        color::yellow);
    world()->add_entity(rendereable{},
                        position{start_position.x + line_length, start_position.y + line_length},
                        line{{start_position.x, start_position.y + line_length}, line_thickness},
                        color::blue);
    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y + line_length},
                        line{{start_position.x, start_position.y}, line_thickness},
                        color::green);
    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y},
                        line{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
                        color::purple);
    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y + line_length},
                        line{{start_position.x + line_length, start_position.y}, line_thickness},
                        color::orange);

    start_position.x = 1920 - line_length * 2;

    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y},
                        box{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
                        color::black);

    return true;
}

void draw_game::end() {
    logger::debug("ending draw game");
}
