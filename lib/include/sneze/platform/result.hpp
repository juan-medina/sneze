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

#include "../platform/error.hpp"

namespace sneze {

/**
 * @brief A result type that can be used to return a value or an error
 *
 * This class is a wrapper around std::variant that can be used to return a value
 * or an error. It is similar to std::expected that will be available in C++23 and
 * heavily inspired by Rust's Result type.
 *
 * @code
 * auto divide_by(int num, int by) -> sneze::result<int, sneze::error> {
 *   if(by == 0) {
 *     return sneze::error{"cannot divide by zero"};
 *   }
 *   return num/by;
 * }
 *
 * if(auto [value, error] = divide_by(10, 2).ok(); error) {
 *   std::cout << "Error: " << error->get_message() << std::endl;
 *   return 1;
 * } else {
 *   std::cout << "Result: " << *value << std::endl;
 * }
 *
 * return 0;
 * @endcode
 * @tparam Value The type of the value to return, defaults to bool
 * @tparam Error The type of the error to return, defaults to sneze::error
 * @see sneze::error
 * @see result::ok
 * @see result::ko
 */
template<class Value = bool, class Error = class error>
class result: public std::variant<Value, Error> {
public:
    /**
     * @brief Construct a new result object given a value
     * @param value the value to return
     */
    // cppcheck-suppress noExplicitConstructor
    // NOLINTNEXTLINE(google-explicit-constructor)
    inline result(const Value &value): std::variant<Value, Error>(value) {}

    /**
     * @brief Construct a new result object given a error
     * @param error the error to return
     */
    // cppcheck-suppress noExplicitConstructor
    // NOLINTNEXTLINE(google-explicit-constructor)
    inline result(const Error &error): std::variant<Value, Error>(error) {}

    /**
     * @brief Return if the result has an error
     *
     * @return true if the result has an error
     * @return false if the result has a value
     * @see result::has_value
     * @see result::get_error
     * @see result::ko
     */
    [[maybe_unused]] inline auto has_error() const noexcept {
        return std::holds_alternative<Error>(*this);
    }

    /**
     * @brief Check if the result has a value
     *
     * @return true if the result has a value
     * @return false if the result has an error
     * @see result::has_error
     * @see result::get_value
     * @see result::ok
     */
    [[maybe_unused]] inline auto has_value() const noexcept {
        return std::holds_alternative<Value>(*this);
    }

    /**
     * @brief Get the error
     *
     * @warning this will throw if the result has a value, for non-throwing version
     * use result::ok or result::ko
     *
     * @return the error object
     * @see result::has_error
     * @see result::ko
     */
    [[maybe_unused]] [[nodiscard]] inline auto get_error() const {
        return std::get<Error>(*this);
    }

    /**
     * @brief Get the value
     *
     * @warning this will throw if the result has an error, for a non-throwing
     * version use result::ok or result::ko
     *
     * @return the value object
     * @see result::has_value
     * @see result::ok
     */
    [[maybe_unused]] [[nodiscard]] inline auto get_value() const {
        return std::get<Value>(*this);
    }

    /**
     * @brief Get value or error
     *
     * only one of the two will be present, the other will be std::nullopt, this will
     * never throw and is preferred over result::get_value and result::get_error.
     *
     * @code
     * if(auto [value, error] = do_something().ok(); error) {
     *   std::cout << "Error: " << error->get_message() << std::endl;
     *   return 1;
     * } else {
     *   std::cout << "Result: " << *value << std::endl;
     * }
     * @endcode
     * @return a tuple with an optional value or an optional error
     * @see result::ko
     */
    [[nodiscard]] inline auto ok() const noexcept -> std::tuple<std::optional<Value>, std::optional<Error>> {
        if(has_error()) {
            return {std::nullopt, *std::get_if<Error>(this)};
        }
        return {*std::get_if<Value>(this), std::nullopt};
    };

    /**
     * @brief Get an optional error
     *
     * if the result has a value, this will return std::nullopt, this will never throw and
     * is preferred over result::get_error.
     *
     * @code
     * if(auto error = do_something().ko(); error) {
     *  std::cout << "imuupError: " << error->get_message() << std::endl;
     *  return 1;
     * }
     * @endcode
     * @return an optional with the error object
     * @see result::ok
     */
    [[nodiscard]] inline auto ko() const noexcept -> std::optional<Error> {
        if(has_error()) {
            return *std::get_if<Error>(this);
        }
        return std::nullopt;
    };
};

} // namespace sneze
