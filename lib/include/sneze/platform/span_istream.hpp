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

#include <cstring>
#include <iostream>
#include <span>
#include <streambuf>

namespace sneze {

class span_stream_buffer: public std::streambuf {
public:
    explicit span_stream_buffer(std::span<const std::byte> span);
};

/**
 * @brief A input stream that reads from a span
 *
 * This allows to read from a span as if it was a file, this is useful for
 * embedded files.
 */
class span_istream: public std::istream {
public:
    /**
     * @brief Construct a new span_istream object
     *
     * @param span The span to read from
     */
    explicit span_istream(std::span<const std::byte> span)
        : std::istream(&span_stream_buffer_), span_stream_buffer_{span} {}

private:
    span_stream_buffer span_stream_buffer_;
};

} // namespace sneze