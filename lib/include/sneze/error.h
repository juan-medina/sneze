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
#include <vector>

namespace sneze {

    class error {
    public:
        error( const std::string& message ): // NOLINT(google-explicit-constructor,modernize-pass-by-value)
            message_{ message } {}

        error( const error& other ) {
            message_ = other.message_;
            causes_ = other.causes_;
        };

        error( const std::string& message, const error& other ) { // NOLINT(google-explicit-constructor,modernize-pass-by-value)
            message_ = message;
            causes_ = other.causes_;
            causes_.insert( causes_.begin(), other.message_ );
        };

        virtual ~error() noexcept = default;

        [[nodiscard]] inline auto message() const noexcept { return message_; }

        [[nodiscard]] inline auto causes() const noexcept { return causes_; }

    private:
        std::string message_;
        std::vector<std::string> causes_;
    };

} // namespace sneze