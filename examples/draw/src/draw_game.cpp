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

#include <string>

// configure the game, this is called before the game starts, and it is used to configure the game
auto draw_game::configure() -> sneze::config {
    // log that we are configuring the game
    sneze::logger::debug("configure");

    // create the configuration with the size, the clear color and the exit and the full screen toggle keys
    return sneze::config()
        .size(logical_width, logical_height)
        .clear(sneze::components::color::light_gray)
        .exit(sneze::keyboard::key::escape)
        .toggle_full_screen(sneze::keyboard::modifier::alt, sneze::keyboard::key::_return);
}

auto draw_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init draw game");

    // add our static text at the bottom right of the screen
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            text,
            sneze::embedded::regular_font,
            text_size,
            sneze::components::alignment{sneze::components::horizontal::right, sneze::components::vertical::bottom}},
        sneze::components::anchor{sneze::components::horizontal::right, sneze::components::vertical::bottom},
        sneze::components::color::white);

    // add the mouse listeners
    world()->add_listener<sneze::events::mouse_button_down, &draw_game::mouse_button_down>(this);
    world()->add_listener<sneze::events::mouse_button_up, &draw_game::mouse_button_up>(this);
    world()->add_listener<sneze::events::mouse_moved, &draw_game::mouse_moved>(this);

    // all good
    return true;
}

void draw_game::end() {
    // log that we are ending the game
    sneze::logger::debug("ending draw game");

    // remove all the listeners
    world()->remove_listeners(this);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_button_down(const sneze::events::mouse_button_down &event) {
    // if the button is the left button
    if(event.button == sneze::mouse::button::left) {
        // remove all the entities that have the drawing tag, if any
        event.world->remove_all_tagged<drawing_tag>();

        // add a new line entity with the drawing tag where the mouse is
        auto entity = event.world->add_entity(sneze::components::renderable{},
                                              event.point,
                                              sneze::components::line{event.point, line_thickness},
                                              sneze::components::color::red);
        // tag the entity with the drawing tag
        event.world->tag<drawing_tag>(entity);
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_button_up(const sneze::events::mouse_button_up &event) {
    // if the button is the left button
    if(event.button == sneze::mouse::button::left) {
        // remove the drawing tag from all the entities
        event.world->remove_all_tags<drawing_tag>();
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_moved(const sneze::events::mouse_moved &event) {
    // get all the entities that have the drawing tag and a line component
    for(auto [entity, line]: event.world->get_tagged<drawing_tag, sneze::components::line>()) {
        // update the line to point to the mouse position
        line.to = event.point;
    }
}
