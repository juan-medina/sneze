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

#include "labels_game.hpp"

#include <fmt/format.h>

// configure the game, this is called before the game starts, and it is used to configure the game
auto labels_game::configure() -> sneze::config {
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
auto labels_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init labels game");

    // load custom BMPFont
    if(auto err = load_font(font_name).ko(); err) {
        sneze::logger::error("failed to load game font font");
        return sneze::error("Failed to load game font.", *err);
    }

    // add top left large white label
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            "Top left large white label",
            font_name,
            font_size_large,
            sneze::components::alignment{sneze::components::horizontal::left, sneze::components::vertical::top}},
        sneze::components::position{corner_gap, corner_gap},
        sneze::components::anchor{sneze::components::horizontal::left, sneze::components::vertical::top},
        sneze::components::color::white);

    // add top right small red label
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            "Top right small red label",
            font_name,
            font_size_small,
            sneze::components::alignment{sneze::components::horizontal::right, sneze::components::vertical::top}},
        sneze::components::position{-corner_gap, corner_gap},
        sneze::components::anchor{sneze::components::horizontal::right, sneze::components::vertical::top},
        sneze::components::color::red);

    // add bottom left small blue label
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            "Bottom left small blue label",
            font_name,
            font_size_small,
            sneze::components::alignment{sneze::components::horizontal::left, sneze::components::vertical::bottom}},
        sneze::components::position{corner_gap, -corner_gap},
        sneze::components::anchor{sneze::components::horizontal::left, sneze::components::vertical::bottom},
        sneze::components::color::blue);

    // add bottom right large green label
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            "Bottom right large green label",
            font_name,
            font_size_large,
            sneze::components::alignment{sneze::components::horizontal::right, sneze::components::vertical::bottom}},
        sneze::components::position{-corner_gap, -corner_gap},
        sneze::components::anchor{sneze::components::horizontal::right, sneze::components::vertical::bottom},
        sneze::components::color::green);

    // add center large lime label
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            "Center large pink label",
            font_name,
            font_size_large,
            sneze::components::alignment{sneze::components::horizontal::center, sneze::components::vertical::center}},
        sneze::components::anchor{sneze::components::horizontal::center, sneze::components::vertical::center},
        sneze::components::color::pink);

    // all good
    return true;
}

// we are ending the game
void labels_game::end() {
    // log that we are ending the game
    sneze::logger::debug("end labels game");

    unload_font(font_name);
}
