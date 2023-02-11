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

#include <sneze/base/color.hpp>
#include <sneze/platform/result.hpp>

namespace sneze {
    class render {

    public:
        render() = default;
        ~render() = default;

        render( const render& ) = delete;
        render( render&& ) = delete;

        render& operator=( const render& ) = delete;
        render& operator=( render&& ) = delete;

        [[maybe_unused]] void clear_color( const color& color ) { clear_color_ = color; }

        [[nodiscard]] result<>
        init( const std::int64_t& width, const std::int64_t& height, const std::string& title, const color& color );

        [[nodiscard]] result<> want_to_close() const;

        void end();

        void begin_frame();

        void end_frame();

    private:
        color clear_color_ = color::Black;
    };
} // namespace sneze