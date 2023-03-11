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

draw_game::draw_game(): application("sneze", "Draw Game") {}

namespace logger = sneze::logger;
using config = sneze::config;
namespace components = sneze::components;
using color = components::color;
using button = sneze::mouse::button;
using color = components::color;

using namespace std::string_literals;
const auto regular_font = "resources/fonts/tilt_warp.fnt"s;

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

    using error = sneze::error;

    if(auto err = load_font(regular_font).ko()) {
        logger::error("game can't load regular font: {}", regular_font);
        return error("Can't load regular font.", *err);
    }

    using renderable = components::renderable;
    using label = components::label;

    using alignment = components::alignment;
    using vertical = components::vertical;
    using horizontal = components::horizontal;
    using anchor = components::anchor;

    static constexpr auto text = "Drawing lines, press left mouse button to start and stop drawing.";

    world()->add_entity(renderable{},
                        label{text, regular_font, 40.F, alignment{horizontal::right, vertical::bottom}},
                        anchor{horizontal::right, vertical::bottom},
                        color::white);

    world()->add_listener<events::mouse_button_down, &draw_game::mouse_button_down>(this);
    world()->add_listener<events::mouse_button_up, &draw_game::mouse_button_up>(this);
    world()->add_listener<events::mouse_moved, &draw_game::mouse_moved>(this);

    return true;
}

void draw_game::end() {
    logger::debug("ending draw game");

    world()->remove_listeners(this);

    unload_font(regular_font);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_button_down(const events::mouse_button_down &event) {
    if(event.button != button::left) {
        return;
    }

    event.world->remove_all_tagged<drawing>();

    using renderable = components::renderable;
    using line = components::line;

    auto entity = event.world->add_entity(renderable{}, event.point, line{event.point, 5.F}, color::red);
    event.world->tag<drawing>(entity);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_button_up(const events::mouse_button_up &event) {
    if(event.button != button::left) {
        return;
    }

    event.world->remove_all_tags<drawing>();
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void draw_game::mouse_moved(const events::mouse_moved &event) {
    using line = components::line;
    for(auto &&[entity, line]: event.world->tagged<drawing, line>()) {
        line.to = event.point;
    }
}
