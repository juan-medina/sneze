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

#include "sneze/components/renderable.hpp"

namespace sneze::components {
[[maybe_unused]] const color color::white = color::rgb(255, 255, 255);
[[maybe_unused]] const color color::black = color::rgb(0, 0, 0);
[[maybe_unused]] const color color::gray = color::rgb(128, 128, 128);
[[maybe_unused]] const color color::light_gray = color::rgb(200, 200, 200);
[[maybe_unused]] const color color::dark_gray = color::rgb(80, 80, 80);
[[maybe_unused]] const color color::yellow = color::rgb(253, 249, 0);
[[maybe_unused]] const color color::gold = color::rgb(255, 203, 0);
[[maybe_unused]] const color color::orange = color::rgb(255, 161, 0);
[[maybe_unused]] const color color::pink = color::rgb(255, 109, 194);
[[maybe_unused]] const color color::red = color::rgb(230, 41, 55);
[[maybe_unused]] const color color::maroon = color::rgb(190, 33, 55);
[[maybe_unused]] const color color::green = color::rgb(0, 228, 48);
[[maybe_unused]] const color color::lime = color::rgb(0, 158, 47);
[[maybe_unused]] const color color::dark_green = color::rgb(0, 117, 44);
[[maybe_unused]] const color color::sky_blue = color::rgb(102, 191, 255);
[[maybe_unused]] const color color::blue = color::rgb(0, 121, 241);
[[maybe_unused]] const color color::dark_blue = color::rgb(0, 82, 172);
[[maybe_unused]] const color color::purple = color::rgb(200, 122, 255);
[[maybe_unused]] const color color::violet = color::rgb(135, 60, 190);
[[maybe_unused]] const color color::dark_purple = color::rgb(112, 31, 126);
[[maybe_unused]] const color color::beige = color::rgb(211, 176, 131);
[[maybe_unused]] const color color::brown = color::rgb(127, 106, 79);
[[maybe_unused]] const color color::dark_brown = color::rgb(76, 63, 47);
[[maybe_unused]] const color color::magenta = color::rgb(231, 72, 86);
[[maybe_unused]] const color color::untinted = color::rgb(255, 255, 255);

std::uint64_t renderable::creation_id_counter = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

} // namespace sneze::components
