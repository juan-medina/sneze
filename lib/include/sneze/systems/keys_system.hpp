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

#include "../events/events.hpp"

#include "system.hpp"

namespace sneze {

class keys_system: public system {
public:
    keys_system(const keyboard::key_mod &exit_key, const keyboard::key_mod &toggle_full_screen_key)
        : exit_key_(exit_key), toggle_full_screen_key_(toggle_full_screen_key) {}

    void init(sneze::world *world) override;

    void end(sneze::world *world) override;

    void update(sneze::world *world) override;

private:
    void key_up(const events::key_up &event);

    static constexpr auto check_key(const events::key_up &event, const keyboard::key_mod &key) -> bool {
        if(key.key_code == keyboard::key::unknown) {
            return false;
        } else if(key.key_mod == keyboard::modifier::none) {
            return event.key == key.key_code;
        } else {
            return (event.key == key.key_code) && (event.modifier & key.key_mod);
        }
    }

    keyboard::key_mod exit_key_;
    keyboard::key_mod toggle_full_screen_key_;
};

} // namespace sneze