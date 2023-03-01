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

namespace logger = sneze::logger;
using config = sneze::config;
namespace components = sneze::components;
using color = components::color;

using namespace std::string_literals;
const auto regular_font = "resources/fonts/tilt_warp.fnt"s;
const auto mono_font = "resources/fonts/fira_mono.fnt"s;

auto example_game::configure() -> config {
    logger::debug("configure");

    namespace keyboard = sneze::keyboard;
    using key = keyboard::key;
    using modifier = keyboard::modifier;

    return config()
        .width(1920)
        .height(1080)
        .clear(color::light_gray)
        .exit(key::escape)
        .toggle_full_screen(modifier::alt, key::_return);
}

auto example_game::init() -> sneze::result<> {
    using error = sneze::error;

    logger::debug("init");

    if(auto err = load_font(regular_font).ko()) {
        logger::error("game can't load regular font: {}", regular_font);
        return error("Can't load regular font.", *err);
    }

    if(auto err = load_font(mono_font).ko()) {
        logger::error("game can't load mono font: {}", mono_font);
        return error("Can't load mono font.", *err);
    }

    auto visits = get_app_setting("visits", std::int64_t{0}) + 1;
    set_app_setting("visits", visits);

    using renderable = components::renderable;
    using label = components::label;
    using position = components::position;

    const auto font_size = 40.f;
    const auto pos_x = 190.f;
    const auto pos_y = 200.f;
    const auto gap_y = font_size + 10.f;
    auto current_y = pos_y;

    const auto counter_1 = 20000;
    const auto counter_2 = 10000;

    world()->add_entity(renderable{},
                        label{fmt::format("Hello World for the {} time!", visits), regular_font, font_size},
                        position{pos_x, current_y += gap_y},
                        color::white);

    world()->add_entity(renderable{},
                        counter{counter_1},
                        label{"Counter:", mono_font, font_size},
                        position{pos_x, current_y += gap_y},
                        color::red);

    world()->add_entity(renderable{},
                        counter{counter_2},
                        label{"Counter:", mono_font, font_size},
                        position{pos_x, current_y += gap_y},
                        color::blue);

    world()->set_global<acceleration>(5);

    world()->add_system<counter_system>();

    return true;
}

void example_game::end() {
    logger::debug("end");

    world()->remove_system<counter_system>();

    unload_font(regular_font);
    unload_font(mono_font);
}
