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

#include "example_game.hpp"

#include <string>

#include <fmt/format.h>

#include "counter_system.hpp"

example_game::example_game(): application("sneze", "Example Game") {}

namespace components = sneze::components;
using color = sneze::components::color;
using config = sneze::config;

using namespace std::string_literals;
const auto regular_font = "resources/fonts/tilt_warp.fnt"s;
const auto mono_font = "resources/fonts/fira_mono.fnt"s;

auto example_game::configure() -> config {
    sneze::logger::debug("configure");
    return config().clear_color(color::LightGray);
}

auto example_game::init() -> sneze::result<> {
    sneze::logger::debug("init");

    if(auto err = load_font(regular_font).ko()) {
        sneze::logger::error("game can't load regular font: {}", regular_font);
        return sneze::error("Can't load regular font.", *err);
    }

    if(auto err = load_font(mono_font).ko()) {
        sneze::logger::error("game can't load mono font: {}", mono_font);
        return sneze::error("Can't load mono font.", *err);
    }

    const auto font_size = 40.f;
    const auto pos_x = 190.f;
    const auto pos_y = 200.f;
    const auto gap_y = font_size + 10.f;
    auto current_y = pos_y;

    const auto counter_1 = 0;
    const auto counter_2 = 10000;

    auto visits = get_app_setting("visits", std::int64_t{0});
    visits++;
    set_app_setting("visits", visits);

    world().create(components::renderable{},
                   components::label{fmt::format("Hello World for the {} time!", visits), regular_font, font_size},
                   components::position{pos_x, current_y += gap_y},
                   color::White);

    world().create(components::renderable{},
                   counter{counter_1},
                   components::label{"Counter:", mono_font, font_size},
                   components::position{pos_x, current_y += gap_y},
                   color::Red);

    world().create(components::renderable{},
                   counter{counter_2},
                   components::label{"Counter:", mono_font, font_size},
                   components::position{pos_x, current_y += gap_y},
                   color::Blue);

    world().global(acceleration{1});

    world().add_system<counter_system>();

    return true;
}

void example_game::end() {
    sneze::logger::debug("end");
    world().remove_system<counter_system>();
    unload_font(regular_font);
    unload_font(mono_font);
}
