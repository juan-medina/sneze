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
class hello_game: public sneze::application {
public:
    // create the application using the name of the team, and the name of the application
    // it will be used to create the window, and for saving the settings
    hello_game(): sneze::application(team_name, game_name) {}

    ~hello_game() override = default;

    hello_game(const hello_game &) = delete;
    hello_game(const hello_game &&) = delete;

    auto operator=(const hello_game &) -> hello_game & = delete;
    auto operator=(const hello_game &&) -> hello_game & = delete;

    [[nodiscard]] auto configure() -> sneze::config override;

    auto init() -> sneze::result<> override;

    void end() override;

private:
    // this is the name of the team
    static constexpr auto team_name = "Sneze";
    // this is the name of the game
    static constexpr auto game_name = "Hello Example";

    // this is the logical width of the game
    static constexpr auto logical_width = 1920;
    // this is the logical height of the game
    static constexpr auto logical_height = 1080;

    // this is our text
    static constexpr auto text = "Hello from Sneze!";
    // this is the size of the text
    static constexpr auto text_size = 60.F;
};
