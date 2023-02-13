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

example_game::example_game(): application( "sneze", "Example Game" ) {}

config example_game::init() {
    logger::debug( "init" );
    get_set_app_setting<std::int64_t>( "visits", 0LL, []( auto visits ) { return visits + 1LL; } );

    world().create( components::renderable{},
                    components::text{ "Hello", 40.f },
                    components::position{ 190.f, 200.f },
                    color::White );

    world().create( components::renderable{},
                    components::text{ "World!", 40.f },
                    components::position{ 190.f, 250.f },
                    color::Yellow );

    return config().clear_color( color::Black );
}

void example_game::end() { logger::debug( "end" ); }
