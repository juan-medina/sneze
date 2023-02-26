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

#include "sneze/systems/sdl_events_system.hpp"

#include "sneze/app/world.hpp"
#include "sneze/events/events.hpp"
#include "sneze/platform/logger.hpp"

#include <SDL.h>

namespace sneze {
void sneze::sdl_events_system::init(sneze::world *) {
    logger::debug("init event system");
}

void sneze::sdl_events_system::end(sneze::world *) {
    logger::debug("end event system");
}

void sneze::sdl_events_system::update(sneze::world *world) {
    SDL_Event eventData;
    while(SDL_PollEvent(&eventData)) {
        switch(eventData.type) {
        case SDL_QUIT:
            world->emmit<events::application_want_closing>();
            break;
        case SDL_KEYDOWN:
            world->emmit<events::key_down>(eventData.key.keysym.sym, eventData.key.keysym.mod);
            break;
        case SDL_KEYUP:
            world->emmit<events::key_up>(eventData.key.keysym.sym, eventData.key.keysym.mod);
            break;
        }
    }
}

} // namespace sneze
