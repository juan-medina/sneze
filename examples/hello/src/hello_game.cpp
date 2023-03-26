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

#include "hello_game.hpp"

// configure the game, this is called before the game starts, and it is used to configure the game
auto hello_game::configure() -> sneze::config {
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
auto hello_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init hello game");

    // add the hello world entity to the world, is in the center of the screen, with a label, using an embedded font
    // and has a blinking color effect
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            text,
            sneze::embedded::regular_font,
            text_size,
            sneze::components::alignment{sneze::components::horizontal::center, sneze::components::vertical::center}},
        sneze::components::anchor{sneze::components::horizontal::center, sneze::components::vertical::center},
        sneze::components::color::white,
        sneze::effects::alternate_color{sneze::components::color::white, sneze::components::color::red});

    // all good
    return true;
}

// we are ending the game
void hello_game::end() {
    // log that we are ending the game
    sneze::logger::debug("ending hello game");
}
