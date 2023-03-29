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
class draw_game: public sneze::application {
public:
    // create the application using the name of the team, and the name of the application
    // it will be used to create the window, and for saving the settings
    draw_game(): sneze::application(team_name, game_name) {}

    ~draw_game() override = default;

    draw_game(const draw_game &) = delete;
    draw_game(const draw_game &&) = delete;

    auto operator=(const draw_game &) -> draw_game & = delete;
    auto operator=(const draw_game &&) -> draw_game & = delete;

    [[nodiscard]] auto configure() -> sneze::config override;

    auto init() -> sneze::result<> override;

    void end() override;

private:
    // this is the name of the team
    static constexpr auto team_name = "Sneze";
    // this is the name of the game
    static constexpr auto game_name = "Draw Example";

    // this is the logical width of the game
    static constexpr auto logical_width = 1920;
    // this is the logical height of the game
    static constexpr auto logical_height = 1080;

    // this is the text to be displayed at the bottom-right of the screen
    static constexpr auto text = "Drawing lines, press left mouse button to start and stop drawing.";
    // this is the size of the text
    static constexpr auto text_size = 40.F;

    // the line thickness
    static constexpr auto line_thickness = 5.F;

    // this is the tag that we will use to identify the line that we are drawing
    struct drawing_tag {};

    // mouse button down listener
    void mouse_button_down(const sneze::events::mouse_button_down &event);

    // mouse button up listener
    void mouse_button_up(const sneze::events::mouse_button_up &event);

    // mouse moved listener
    void mouse_moved(const sneze::events::mouse_moved &event);
};
