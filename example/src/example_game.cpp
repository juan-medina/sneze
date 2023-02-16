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

#include "counter_system.hpp"

example_game::example_game(): application("sneze", "Example Game") {}

auto example_game::init() -> sneze::config {
    sneze::logger::debug("init");
    get_set_app_setting<std::int64_t>("visits", 0LL, [](auto visits) { return visits + 1LL; });

    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    world().create(sneze::components::renderable{}, sneze::components::label{"Hello", 40.f},
                   sneze::components::position{190.f, 200.f}, sneze::color::White);

    world().create(sneze::components::renderable{}, sneze::components::label{"World!", 40.f},
                   sneze::components::position{190.f, 250.f}, sneze::color::Yellow);

    world().create(sneze::components::renderable{}, counter{}, sneze::components::label{"Counter:", 40.f},
                   sneze::components::position{190.f, 300.f}, sneze::color::Red);

    world().create(sneze::components::renderable{}, counter{10000}, sneze::components::label{"Counter:", 40.f},
                   sneze::components::position{190.f, 350.f}, sneze::color::Blue);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

    world().add_system<counter_system>();

    return sneze::config().clear_color(sneze::color::Black);
}

void example_game::end() {
    sneze::logger::debug("end");
    world().remove_system<counter_system>();
}
