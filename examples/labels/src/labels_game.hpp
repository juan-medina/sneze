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

class labels_game: public sneze::application {
public:
    // create the application using the name of the team, and the name of the application
    // it will be used to create the window, and for saving the settings
    labels_game(): sneze::application(team_name, game_name) {}

    ~labels_game() override = default;

    labels_game(const labels_game &) = delete;
    labels_game(const labels_game &&) = delete;

    auto operator=(const labels_game &) -> labels_game & = delete;
    auto operator=(const labels_game &&) -> labels_game & = delete;

    [[nodiscard]] auto configure() -> sneze::config override;

    auto init() -> sneze::result<> override;

    void end() override;

private:
    // this is the name of the team
    static constexpr auto team_name = "Sneze";
    // this is the name of the game
    static constexpr auto game_name = "Labels Example";

    // this is the logical width of the game
    static constexpr auto logical_width = 1920;
    // this is the logical height of the game
    static constexpr auto logical_height = 1080;

    // the font name
    static constexpr auto font_name = "resources/fonts/mystery_quest.fnt";

    // this is the font size for the small text
    static constexpr auto font_size_small = 60.F;
    // this is the font size for the large text
    static constexpr auto font_size_large = 90.F;

    // corner gap for font placement
    static constexpr auto corner_gap = 10.F;
};
