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

#pragma once

#include <sneze/sneze.hpp>

// this is the main class for the game, it inherits from sneze::application
class sprites_game: public sneze::application {
public:
    // create the application using the name of the team, and the name of the application
    // it will be used to create the window, and for saving the settings
    sprites_game(): sneze::application(team_name, game_name) {}

    ~sprites_game() override = default;

    sprites_game(const sprites_game &) = delete;
    sprites_game(const sprites_game &&) = delete;

    auto operator=(const sprites_game &) -> sprites_game & = delete;
    auto operator=(const sprites_game &&) -> sprites_game & = delete;

    [[nodiscard]] auto configure() -> sneze::config override;

    auto init() -> sneze::result<> override;

    void end() override;

private:
    // this is the name of the team
    static constexpr auto team_name = "Sneze";
    // this is the name of the game
    static constexpr auto game_name = "Sprites Example";

    // this is the logical width of the game
    static constexpr auto logical_width = 1920;
    // this is the logical height of the game
    static constexpr auto logical_height = 1080;

    // this is our ghost sprite sheet
    static constexpr auto ghost_sprite_sheet = "resources/sprites/kawaii-ghost/kawaii-ghost.json";
    // this is the default frame for our ghost
    static constexpr auto ghost_default_frame = "default.png";
    // this is the happy frame for our ghost
    static constexpr auto ghost_happy_frame = "happy.png";
    // this is the damaged frame for our ghost
    static constexpr auto ghost_damaged_frame = "damaged.png";

    // this is our shadow sprite, it is a single image
    static constexpr auto shadow_sprite = "resources/sprites/shadow.png";
    static constexpr auto shadow_gap = 80.F;

    // this is the text that will be displayed at the bottom of the screen
    static constexpr auto bottom_text = "Space to change sprite, cursors to flip direction.";
    // this is the size of the text
    static constexpr auto text_size = 40.F;

    // this is the tag for our ghost
    struct ghost_tag {};

    // on key up event
    void on_key_up(const sneze::events::key_up &event);

    // on key down event
    void on_key_down(const sneze::events::key_down &event);
};
