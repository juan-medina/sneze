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

#include <string>

#include "../components/renderable.hpp"
#include "../device/keyboard.hpp"
#include "../embedded/embedded.hpp"

namespace sneze {

class config {
public:
    explicit config() = default;
    ~config() = default;

    config(const config &) = default; // NOLINT(bugprone-exception-escape)
    config(config &&) = default;

    auto operator=(const config &) -> config & = default;
    auto operator=(config &&) -> config & = default;

    [[maybe_unused]] [[nodiscard]] auto clear(const components::color &clear) -> config {
        clear_ = clear;
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::code &key) -> config {
        exit_ = {key};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::mod &modifier, const keyboard::code &key) -> config {
        exit_ = {key, modifier};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::code &key) -> config {
        toggle_full_screen_ = {key};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::mod &modifier, const keyboard::code &key)
        -> config {
        toggle_full_screen_ = {key, modifier};
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto window(int width, int height) -> config {
        window_.width = static_cast<float>(width);
        window_.height = static_cast<float>(height);
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto logical(int width, int height) -> config {
        logical_.width = static_cast<float>(width);
        logical_.height = static_cast<float>(height);
        return *this;
    }

    [[maybe_unused]] [[nodiscard]] auto size(int width, int height) -> config {
        return window(width, height).logical(width, height);
    }

    [[maybe_unused]] [[nodiscard]] auto icon(const std::string &icon) -> config {
        icon_ = icon;
        return *this;
    }

    [[nodiscard]] inline auto clear() const -> const auto & {
        return clear_;
    }

    [[nodiscard]] inline auto exit() const -> const auto & {
        return exit_;
    }

    [[nodiscard]] inline auto toggle_full_screen() const -> const auto & {
        return toggle_full_screen_;
    }

    [[nodiscard]] inline auto window() const -> const auto & {
        return window_;
    }

    [[nodiscard]] inline auto logical() const -> const auto & {
        return logical_;
    }

    [[nodiscard]] inline auto icon() const -> const auto & {
        return icon_;
    }

private:
    components::size window_ = {1920, 1080};
    components::size logical_ = {1920, 1080};
    components::color clear_ = components::color::black;
    keyboard::key_modifier exit_ = {keyboard::key::unknown, keyboard::modifier::none};
    keyboard::key_modifier toggle_full_screen_ = {keyboard::key::unknown, keyboard::modifier::none};
    std::string icon_ = embedded::sneze_logo;
};

} // namespace sneze
