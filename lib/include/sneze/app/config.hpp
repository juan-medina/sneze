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

#include "../components/renderable.hpp"
#include "../device/keyboard.hpp"

namespace sneze {

class config {
public:
    explicit config() = default;
    ~config() = default;

    config(const config &) = default;
    config(config &&) = default;

    auto operator=(const config &) -> config & = default;
    auto operator=(config &&) -> config & = default;

    [[maybe_unused]] [[nodiscard]] auto clear_color(const components::color &clear_color) -> config {
        clear_color_ = clear_color;
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::code &key) -> config {
        exit_key_ = {key, keyboard::modifier::none};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::mod &modifier, const keyboard::code &key) -> config {
        exit_key_ = keyboard::key_mod{key, modifier};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::code &key) -> config {
        toggle_full_screen_key_ = {key, keyboard::modifier::none};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::mod &modifier, const keyboard::code &key)
        -> config {
        toggle_full_screen_key_ = keyboard::key_mod{key, modifier};
        return *this;
    }

    [[nodiscard]] inline auto clear_color() const -> const auto & {
        return clear_color_;
    }

    [[nodiscard]] inline auto exit() const -> const auto & {
        return exit_key_;
    }

    [[nodiscard]] inline auto toggle_full_screen() const -> const auto & {
        return toggle_full_screen_key_;
    }

private:
    components::color clear_color_ = components::color::Black;
    keyboard::key_mod exit_key_ = {keyboard::key::unknown, keyboard::modifier::none};
    keyboard::key_mod toggle_full_screen_key_ = {keyboard::key::unknown, keyboard::modifier::none};
};

} // namespace sneze
