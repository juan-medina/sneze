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

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace sneze {


/**
 * @brief Get the raw type name
 *
 * @tparam Type The type to get the name from
 * @return The raw type name
 */
template<typename Type>
[[nodiscard]] constexpr auto raw_type_name() -> std::string_view {
#if not defined(_MSC_VER)
    return {static_cast<const char *>(__PRETTY_FUNCTION__)};
#else
    return {static_cast<const char *>(__FUNCSIG__)};
#endif
}

//! Information of the format of a type name
struct type_name_format {
    //! The number of characters to remove from the beginning of the type name
    std::size_t junk_leading = 0;
    //! The total number of characters to remove from the type name
    std::size_t junk_total = 0;
};

//! Holds the format of the type name for the current compiler
constexpr type_name_format type_name_format_arr = [] {
    type_name_format ret;
    std::string_view const sample = raw_type_name<int>();
    ret.junk_leading = sample.find("int");
    ret.junk_total = sample.size() - 3;
    return ret;
}();
static_assert(type_name_format_arr.junk_leading != static_cast<std::size_t>(-1),
              "Unable to determine the type name format on this compiler.");

/**
 * @brief Get the type name storage
 * @tparam Type the type to get information from
 */
template<typename Type>
static constexpr auto type_name_storage = [] {
    std::array<char, raw_type_name<Type>().size() - type_name_format_arr.junk_total + 1> ret{};
    std::copy_n(raw_type_name<Type>().data() + type_name_format_arr.junk_leading, ret.size() - 1, ret.data());
    return ret;
}();

/**
 * @brief Get the type name as an string for a given type.
 *
 * @code
 * std::cout << type_name<int>() << std::endl;
 * @endcode
 * this will print "int" on most compilers.
 *
 * @tparam Type The type to get the name of.
 *
 * @return A string with the type name.
 */
template<typename Type>
[[nodiscard]] constexpr auto type_name() -> std::string_view {
    return {type_name_storage<Type>.data(), type_name_storage<Type>.size() - 1};
}

} // namespace sneze