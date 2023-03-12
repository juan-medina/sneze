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

#include "shapes_game.hpp"

shapes_game::shapes_game(): application("sneze", "Shapes Game") {}

namespace logger = sneze::logger;
using config = sneze::config;
namespace components = sneze::components;
using color = components::color;

auto shapes_game::configure() -> config {
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

auto shapes_game::init() -> result {
    logger::debug("init shapes game");

    using rendereable = components::renderable;
    using line = components::line;
    using box = components::box;
    using solid_box = components::solid_box;
    using border_box = components::border_box;
    using position = components::position;

    const auto line_thickness = 15.F;
    const auto line_length = 400.F;
    auto start_position = position{line_length, (1080.F / 4.F) - (line_length / 2.F)};

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

    start_position.x = 1920.F - line_length * 2.F;

    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y},
                        solid_box{start_position.x + line_length, start_position.y + line_length},
                        color::pink);

    start_position.x = line_length;
    start_position.y = 1080.F - (line_length * 1.25F);

    world()->add_entity(rendereable{},
                        position{start_position.x, start_position.y},
                        box{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
                        color::dark_purple);

    start_position.x = 1920.F - line_length * 2.F;

    world()->add_entity(
        rendereable{},
        position{start_position.x, start_position.y},
        border_box{{start_position.x + line_length, start_position.y + line_length}, line_thickness, color::maroon},
        color::sky_blue);

    return true;
}

void shapes_game::end() {
    logger::debug("ending shapes game");
}
