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

#include <optional>
#include <variant>

#include "error.h"

namespace sneze {

    template <class Value = bool, class Error = class error>
    class result : public std::variant<Value, Error> {
    public:
        inline result( const Value& value ): // NOLINT(google-explicit-constructor)
            std::variant<Value, Error>( value ) {}

        inline result( const Error& err ): // NOLINT(google-explicit-constructor)
            std::variant<Value, Error>( err ) {}

        inline bool has_error() { return std::holds_alternative<Error>( *this ); }

        [[maybe_unused]] inline bool has_value() { return std::holds_alternative<Value>( *this ); }

        [[maybe_unused]] [[nodiscard]] inline Error error() const noexcept { return std::get<Error>( *this ); }

        [[maybe_unused]] [[nodiscard]] inline Value value() const noexcept { return std::get<Value>( *this ); }

        inline std::tuple<std::optional<Value>, std::optional<Error>> ok() {
            if ( has_error() ) {
                return { std::nullopt, error() };
            } else {
                return { value(), std::nullopt };
            }
        };

        inline std::optional<Error> ko() {
            if ( has_error() ) {
                return error();
            } else {
                return std::nullopt;
            }
        };
    };

} // namespace sneze