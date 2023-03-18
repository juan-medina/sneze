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
#include <utility>
#include <vector>

namespace sneze {

/**
 * @brief Represents an error with a message and a list of causes
 */
class error {
public:
    /**
     * @brief Construct a new error object
     *
     * @param message The error message
     */
    explicit error(std::string message): message_{std::move(message)} {}

    /**
     * @brief Construct a new error object, with a cause error
     *
     * @param message The error message
     * @param other The error that caused this error
     */
    error(std::string message, const error &other): message_{std::move(message)}, causes_{other.causes_} {
        causes_.insert(causes_.begin(), other.message_);
    };

    /**
     * @brief Get the error message
     * @return the error message
     */
    [[nodiscard]] inline auto get_message() const -> const std::string & {
        return message_;
    }

    /**
     * @brief Get the causes of this error
     * @return the causes of this error
     */
    [[nodiscard]] inline auto get_causes() const -> const std::vector<std::string> & {
        return causes_;
    }

private:
    /**
     * @brief Holds the error message
     */
    std::string message_;

    /**
     * @brief Holds the causes of this error
     */
    std::vector<std::string> causes_{};
};

} // namespace sneze
