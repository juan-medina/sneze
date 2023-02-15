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

#include "sneze/platform/error.hpp"

#include <optional>
#include <variant>

namespace sneze {

template<class Value = bool, class Error = class error>
class result: public std::variant<Value, Error> {
public:
    // cppcheck-suppress noExplicitConstructor
    // NOLINTNEXTLINE(google-explicit-constructor)
    inline result(const Value &value): std::variant<Value, Error>(value) {}

    // cppcheck-suppress noExplicitConstructor
    // NOLINTNEXTLINE(google-explicit-constructor)
    inline result(const Error &err): std::variant<Value, Error>(err) {}

    inline auto has_error() {
        return std::holds_alternative<Error>(*this);
    }

    [[maybe_unused]] inline auto has_value() {
        return std::holds_alternative<Value>(*this);
    }

    [[maybe_unused]] [[nodiscard]] inline auto error() const noexcept {
        return std::get<Error>(*this);
    }

    [[maybe_unused]] [[nodiscard]] inline auto value() const noexcept {
        return std::get<Value>(*this);
    }

    inline auto ok() -> std::tuple<std::optional<Value>, std::optional<Error>> {
        if(has_error()) {
            return {std::nullopt, error()};
        } else {
            return {value(), std::nullopt};
        }
    };

    inline auto ko() -> std::optional<Error> {
        if(has_error()) {
            return error();
        } else {
            return std::nullopt;
        }
    };
};

} // namespace sneze