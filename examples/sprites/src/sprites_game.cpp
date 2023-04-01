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

// configure the game, this is called before the game starts, and it is used to configure the game
auto sprites_game::configure() -> sneze::config {
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
auto sprites_game::init() -> sneze::result<> {
    // log that we are initializing the game
    sneze::logger::debug("init sprites game");

    // load the ghost sprite sheet, containing the ghost frames
    if(load_sprite_sheet(ghost_sprite_sheet).ko()) {
        sneze::logger::error("game can't load ghost sprite sheet");
        return sneze::error("Can't load ghost sprite sheet.");
    }

    // load the shadow sprite, is a single sprite without sheet
    if(load_sprite(shadow_sprite).ko()) {
        sneze::logger::error("game can't load shadow sprite");
        return sneze::error("Can't load shadow sprite.");
    }

    // add the shadow entity to the world, it is a single sprite, we set it first to be rendered to be behind the ghost
    world()->add_entity(sneze::components::renderable{},
                        sneze::components::sprite{shadow_sprite},
                        sneze::components::position{logical_width / 2.F, (logical_height / 2.F) + shadow_gap},
                        sneze::components::color::untinted);

    // add the ghost entity to the world, it is contained in the sprite sheet
    auto ghost = world()->add_entity(sneze::components::renderable{},
                                     sneze::components::sprite{ghost_sprite_sheet, ghost_default_frame},
                                     sneze::components::position{logical_width / 2.F, logical_height / 2.F},
                                     sneze::components::color::untinted);

    // tag the ghost entity
    world()->tag<ghost_tag>(ghost);

    // add the bottom text to the world
    world()->add_entity(
        sneze::components::renderable{},
        sneze::components::label{
            bottom_text,
            sneze::embedded::regular_font,
            text_size,
            sneze::components::alignment{sneze::components::horizontal::right, sneze::components::vertical::bottom}},
        sneze::components::anchor{sneze::components::horizontal::right, sneze::components::vertical::bottom},
        sneze::components::color::white);

    // listen for key up and key down events
    world()->add_listener<sneze::events::key_up, &sprites_game::on_key_up>(this);
    world()->add_listener<sneze::events::key_down, &sprites_game::on_key_down>(this);

    // all good
    return true;
}

// we are ending the game
void sprites_game::end() {
    // log that we are ending the game
    sneze::logger::debug("ending sprites game");

    // unload the ghost sprite sheet
    unload_sprite_sheet(ghost_sprite_sheet);

    // unload the shadow sprite
    unload_sprite(shadow_sprite);

    // remove the listeners
    world()->remove_listeners(this);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void sprites_game::on_key_up(const sneze::events::key_up &event) {
    // if the space key is pressed
    if(event.key == sneze::keyboard::key::space) {
        // get all the entities with a sprite component and a ghost tag
        for(auto [entity, sprite]: event.world->get_tagged<ghost_tag, sneze::components::sprite>()) {
            // get the current frame
            auto frame = sprite.frame;
            // change the frame
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
void sprites_game::on_key_down(const sneze::events::key_down &event) {
    // not flipped in the x-axis by default
    bool flip_x = false;

    // if the key is the left key
    if(event.key == sneze::keyboard::key::left) {
        flip_x = true;
    }
    // if the key is not the right key
    else if(event.key != sneze::keyboard::key::right) {
        // we don't care about this key
        return;
    }

    // get all the entities with a sprite component and a ghost tag
    for(auto [entity, sprite]: event.world->get_tagged<ghost_tag, sneze::components::sprite>()) {
        // flip the sprite in the x-axis
        sprite.flip_x = flip_x;
    }
}
