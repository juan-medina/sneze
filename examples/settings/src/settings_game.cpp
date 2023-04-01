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

#include "settings_game.hpp"

#include <fmt/format.h>

// configure the game, this is called before the game starts, and it is used to configure the game
auto settings_game::configure() -> sneze::config {
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
auto settings_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init settings game");

    // get the visits an increase it by one
    auto visits = get_app_setting("visits", std::int64_t{0}) + 1;
    set_app_setting("visits", visits);

    // add an entity to the world, this entity will be rendered as a label, this will have the visits counter
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            fmt::format("You seen this {} times!", visits),
            sneze::embedded::regular_font,
            font_size,
            sneze::components::alignment{sneze::components::horizontal::center, sneze::components::vertical::center}},
        sneze::components::anchor{sneze::components::horizontal::center, sneze::components::vertical::center},
        sneze::components::color::white);

    // all good
    return true;
}

// we are ending the game
void settings_game::end() {
    // log that we are ending the game
    sneze::logger::debug("end settings game");
}
