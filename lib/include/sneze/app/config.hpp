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

#include "../components/color.hpp"

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

    [[nodiscard]] inline auto clear_color() const -> const components::color & {
        return clear_color_;
    }

private:
    components::color clear_color_ = components::color::Black;
};

} // namespace sneze
