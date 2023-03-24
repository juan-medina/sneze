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

#include "sprites_game.hpp"

using namespace std::string_literals;

const auto ghost_sprite_sheet = "resources/sprites/kawaii-ghost/kawaii-ghost.json"s;
const auto ghost_default_frame = "default.png"s;
const auto ghost_happy_frame = "happy.png"s;
const auto ghost_damaged_frame = "damaged.png"s;

sprites_game::sprites_game(): application("sneze", "Sprites Game") {}

namespace logger = sneze::logger;
using config = sneze::config;
namespace components = sneze::components;
using color = components::color;

auto sprites_game::configure() -> config {
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

auto sprites_game::init() -> result {
    logger::debug("init sprites game");

    using error = sneze::error;

    if(load_sprite_sheet(ghost_sprite_sheet).ko()) {
        logger::error("game can't load sprite sheet");
        return error("Can't load sprite sheet.");
    }

    using renderable = components::renderable;
    using sprite = components::sprite;
    using position = components::position;

    world()->add_entity(renderable{},
                        sprite{ghost_sprite_sheet, ghost_default_frame},
                        position{1920.F / 2.F, 1080.F / 2.F},
                        color::untinted);

    static constexpr auto text = "Space to change sprite, cursors to flip direction.";

    using label = components::label;
    using alignment = components::alignment;
    using vertical = components::vertical;
    using horizontal = components::horizontal;
    using anchor = components::anchor;
    namespace embedded = sneze::embedded;

    world()->add_entity(renderable{},
                        label{text, embedded::regular_font, 40.F, alignment{horizontal::right, vertical::bottom}},
                        anchor{horizontal::right, vertical::bottom},
                        color::white);

    world()->add_listener<key_up, &sprites_game::on_key_up>(this);
    world()->add_listener<key_down, &sprites_game::on_key_down>(this);

    return true;
}

void sprites_game::end() {
    logger::debug("ending sprites game");

    unload_sprite_sheet(ghost_sprite_sheet);

    world()->remove_listeners(this);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void sprites_game::on_key_up(const key_up &event) {
    if(event.key == sneze::keyboard::key::space) {
        for(auto [entity, sprite]: event.world->get_entities<components::sprite>()) {
            auto frame = sprite.frame;
            if(frame == ghost_default_frame) {
                sprite.frame = ghost_happy_frame;
            } else if(frame == ghost_happy_frame) {
                sprite.frame = ghost_damaged_frame;
            } else if(frame == ghost_damaged_frame) {
                sprite.frame = ghost_default_frame;
            }
        }
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void sprites_game::on_key_down(const key_down &event) {
    if(event.key == sneze::keyboard::key::left) {
        for(auto [entity, sprite]: event.world->get_entities<components::sprite>()) {
            sprite.flip_x = true;
        }
    } else if(event.key == sneze::keyboard::key::right) {
        for(auto [entity, sprite]: event.world->get_entities<components::sprite>()) {
            sprite.flip_x = false;
        }
    }
}
