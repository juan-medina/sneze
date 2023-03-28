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

// configure the game, this is called before the game starts, and it is used to configure the game
auto shapes_game::configure() -> sneze::config {
    // log that we are configuring the game
    sneze::logger::debug("configure");

    // create the configuration with the size, the clear color and the exit and the full screen toggle keys
    return sneze::config()
        .size(logical_width, logical_height)
        .clear(sneze::components::color::light_gray)
        .exit(sneze::keyboard::key::escape)
        .toggle_full_screen(sneze::keyboard::modifier::alt, sneze::keyboard::key::_return);
}

// initialize the game, this is called before the game starts
auto shapes_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init shapes game");

    // we will draw 4 squares, each one one in a different way

    // the start position of the first square
    auto start_position = sneze::components::position{line_length, (1080.F / 4.F) - (line_length / 2.F)};

    // first line
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x, start_position.y},
                        sneze::components::line{{start_position.x + line_length, start_position.y}, line_thickness},
                        sneze::components::color::red);

    // second line
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::position{start_position.x + line_length, start_position.y},
        sneze::components::line{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
        sneze::components::color::yellow);

    // third line
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x + line_length, start_position.y + line_length},
                        sneze::components::line{{start_position.x, start_position.y + line_length}, line_thickness},
                        sneze::components::color::blue);

    // fourth line
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x, start_position.y + line_length},
                        sneze::components::line{{start_position.x, start_position.y}, line_thickness},
                        sneze::components::color::green);

    // first diagonal line
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::position{start_position.x, start_position.y},
        sneze::components::line{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
        sneze::components::color::purple);

    // second diagonal line
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x, start_position.y + line_length},
                        sneze::components::line{{start_position.x + line_length, start_position.y}, line_thickness},
                        sneze::components::color::orange);

    // position for the next square
    start_position.x = 1920.F - line_length * 2.F;

    // add a solid box square, a square with a border but not filled
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x, start_position.y},
                        sneze::components::solid_box{start_position.x + line_length, start_position.y + line_length},
                        sneze::components::color::pink);

    // position for the next square
    start_position.x = line_length;
    start_position.y = 1080.F - (line_length * 1.25F);

    // add box, is a filled square
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::position{start_position.x, start_position.y},
        sneze::components::box{{start_position.x + line_length, start_position.y + line_length}, line_thickness},
        sneze::components::color::dark_purple);

    // position for the next square
    start_position.x = 1920.F - line_length * 2.F;

    // add a border box, is a square with a border and filled
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::position{start_position.x, start_position.y},
                        sneze::components::border_box{{start_position.x + line_length, start_position.y + line_length},
                                                      line_thickness,
                                                      sneze::components::color::maroon},
                        sneze::components::color::sky_blue);

    // all good
    return true;
}

// we are ending the game
void shapes_game::end() {
    // log that we are ending the game
    sneze::logger::debug("ending shapes game");
}
