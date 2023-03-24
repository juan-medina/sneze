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

#include <cinttypes>
#include <string>

//! @brief keyboard namespace
namespace sneze::keyboard {

//! @brief keyboard key codes
using code = int32_t;

/**
 * @brief keyboard key class
 */
class key {
public:
    //! unknown key
    [[maybe_unused]] static const code unknown;
    //! space key
    [[maybe_unused]] static const code a; // NOLINT(readability-identifier-length)
    //! b key
    [[maybe_unused]] static const code b; // NOLINT(readability-identifier-length)
    //! c key
    [[maybe_unused]] static const code c; // NOLINT(readability-identifier-length)
    //! d key
    [[maybe_unused]] static const code d; // NOLINT(readability-identifier-length)
    //! e key
    [[maybe_unused]] static const code e; // NOLINT(readability-identifier-length)
    //! f key
    [[maybe_unused]] static const code f; // NOLINT(readability-identifier-length)
    //! g key
    [[maybe_unused]] static const code g; // NOLINT(readability-identifier-length)
    //! h key
    [[maybe_unused]] static const code h; // NOLINT(readability-identifier-length)
    //! i key
    [[maybe_unused]] static const code i; // NOLINT(readability-identifier-length)
    //! j key
    [[maybe_unused]] static const code j; // NOLINT(readability-identifier-length)
    //! k key
    [[maybe_unused]] static const code k; // NOLINT(readability-identifier-length)
    //! l key
    [[maybe_unused]] static const code l; // NOLINT(readability-identifier-length)
    //! m key
    [[maybe_unused]] static const code m; // NOLINT(readability-identifier-length)
    //! n key
    [[maybe_unused]] static const code n; // NOLINT(readability-identifier-length)
    //! o key
    [[maybe_unused]] static const code o; // NOLINT(readability-identifier-length)
    //! p key
    [[maybe_unused]] static const code p; // NOLINT(readability-identifier-length)
    //! q key
    [[maybe_unused]] static const code q; // NOLINT(readability-identifier-length)
    //! r key
    [[maybe_unused]] static const code r; // NOLINT(readability-identifier-length)
    //! s key
    [[maybe_unused]] static const code s; // NOLINT(readability-identifier-length)
    //! t key
    [[maybe_unused]] static const code t; // NOLINT(readability-identifier-length)
    //! u key
    [[maybe_unused]] static const code u; // NOLINT(readability-identifier-length)
    //! v key
    [[maybe_unused]] static const code v; // NOLINT(readability-identifier-length)
    //! w key
    [[maybe_unused]] static const code w; // NOLINT(readability-identifier-length)
    //! x key
    [[maybe_unused]] static const code x; // NOLINT(readability-identifier-length)
    //! y key
    [[maybe_unused]] static const code y; // NOLINT(readability-identifier-length)
    //! z key
    [[maybe_unused]] static const code z; // NOLINT(readability-identifier-length)
    //! 1 key
    [[maybe_unused]] static const code _1; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 2 key
    [[maybe_unused]] static const code _2; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 3 key
    [[maybe_unused]] static const code _3; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 4 key
    [[maybe_unused]] static const code _4; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 5 key
    [[maybe_unused]] static const code _5; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 6 key
    [[maybe_unused]] static const code _6; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 7 key
    [[maybe_unused]] static const code _7; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 8 key
    [[maybe_unused]] static const code _8; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 9 key
    [[maybe_unused]] static const code _9; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! 0 key
    [[maybe_unused]] static const code _0; // NOLINT(readability-identifier-naming,readability-identifier-length)
    //! return key
    [[maybe_unused]] static const code _return; // NOLINT(readability-identifier-naming)
    //! escape key
    [[maybe_unused]] static const code escape;
    //! backspace key
    [[maybe_unused]] static const code backspace;
    //! tab key
    [[maybe_unused]] static const code tab;
    //! space key
    [[maybe_unused]] static const code space;
    //! minus key
    [[maybe_unused]] static const code minus;
    //! equals key
    [[maybe_unused]] static const code equals;
    //! left bracket key
    [[maybe_unused]] static const code left_bracket;
    //! right bracket key
    [[maybe_unused]] static const code right_bracket;
    //! back slash key
    [[maybe_unused]] static const code back_slash;
    //! hash key
    [[maybe_unused]] static const code non_us_hash;
    //! forward slash key
    [[maybe_unused]] static const code forward_slash;
    //! semicolon key
    [[maybe_unused]] static const code semicolon;
    //! back quote key
    [[maybe_unused]] static const code back_quote;
    //! comma key
    [[maybe_unused]] static const code comma;
    //! period key
    [[maybe_unused]] static const code period;
    //! caps lock key
    [[maybe_unused]] static const code caps_lock;
    //! f1 key
    [[maybe_unused]] static const code f1; // NOLINT(readability-identifier-length)
    //! f2 key
    [[maybe_unused]] static const code f2; // NOLINT(readability-identifier-length)
    //! f3 key
    [[maybe_unused]] static const code f3; // NOLINT(readability-identifier-length)
    //! f4 key
    [[maybe_unused]] static const code f4; // NOLINT(readability-identifier-length)
    //! f5 key
    [[maybe_unused]] static const code f5; // NOLINT(readability-identifier-length)
    //! f6 key
    [[maybe_unused]] static const code f6; // NOLINT(readability-identifier-length)
    //! f7 key
    [[maybe_unused]] static const code f7; // NOLINT(readability-identifier-length)
    //! f8 key
    [[maybe_unused]] static const code f8; // NOLINT(readability-identifier-length)
    //! f9 key
    [[maybe_unused]] static const code f9; // NOLINT(readability-identifier-length)
    //! f10 key
    [[maybe_unused]] static const code f10;
    //! f11 key
    [[maybe_unused]] static const code f11;
    //! f12 key
    [[maybe_unused]] static const code f12;
    //! pause key
    [[maybe_unused]] static const code pause;
    //! exclamation mark key
    [[maybe_unused]] static const code exclamation_mark;
    //! double quote key
    [[maybe_unused]] static const code double_quote;
    //! dollar key
    [[maybe_unused]] static const code dollar;
    //! ampersand key
    [[maybe_unused]] static const code ampersand;
    //! single quote key
    [[maybe_unused]] static const code single_quote;
    //! left parenthesis key
    [[maybe_unused]] static const code left_parenthesis;
    //! right parenthesis key
    [[maybe_unused]] static const code right_parenthesis;
    //! asterisk key
    [[maybe_unused]] static const code asterisk;
    //! plus key
    [[maybe_unused]] static const code plus;
    //! print screen key
    [[maybe_unused]] static const code print_screen;
    //! scroll lock key
    [[maybe_unused]] static const code scroll_lock;
    //! insert key
    [[maybe_unused]] static const code insert;
    //! home key
    [[maybe_unused]] static const code home;
    //! page up key
    [[maybe_unused]] static const code page_up;
    //! supr key
    [[maybe_unused]] static const code supr;
    //! end key
    [[maybe_unused]] static const code end;
    //! page down key
    [[maybe_unused]] static const code page_down;
    //! right key
    [[maybe_unused]] static const code right;
    //! left key
    [[maybe_unused]] static const code left;

    /**
     * @brief Get the string representation of a key code.
     *
     * @param key The key code.
     * @return The string representation of the key code.
     */
    static auto string(const code &key) -> std::string;
};

//! The modifier type.
using mod = uint16_t;

/**
 * @brief The key modifier class.
 */
class modifier {
public:
    //! no modifier
    [[maybe_unused]] static const mod none;
    //! left shift modifier
    [[maybe_unused]] static const mod left_shift;
    //! right shift modifier
    [[maybe_unused]] static const mod right_shift;
    //! left control modifier
    [[maybe_unused]] static const mod left_control;
    //! right control modifier
    [[maybe_unused]] static const mod right_control;
    //! left alt modifier
    [[maybe_unused]] static const mod left_alt;
    //! right alt modifier
    [[maybe_unused]] static const mod right_alt;
    //! left gui modifier, Windows key, Command key (Mac), Super key (Linux)
    [[maybe_unused]] static const mod left_gui;
    //! right gui modifier, Windows key, Command key (Mac), Super key (Linux)
    [[maybe_unused]] static const mod right_gui;
    //! shift modifier
    [[maybe_unused]] static const mod shift;
    //! control modifier
    [[maybe_unused]] static const mod control;
    //! alt modifier
    [[maybe_unused]] static const mod alt;
    //! gui modifier, Windows key, Command key (Mac), Super key (Linux)
    [[maybe_unused]] static const mod gui;

    /**
     * @brief Get the string representation of a modifier.
     *
     * @param modifier The modifier.
     * @return The string representation of the modifier.
     */
    static auto string(const mod &modifier) -> std::string;
};

/**
 * @brief Key and modifier pair.
 */
struct key_modifier {
    //! The key code, or unknown if not a key.
    code key = key::unknown; // NOLINT(misc-non-private-member-variables-in-classes)
    //! The modifier, or none if not a modifier.
    mod modifier = modifier::none; // NOLINT(misc-non-private-member-variables-in-classes)

    /**
     * @brief compare two key_modifier
     * @param other the other key_modifier to compare to
     * @return true if the key_modifier are equal, false otherwise
     */
    constexpr auto operator==(const key_modifier &other) const {
        if(other.modifier == keyboard::modifier::none) {
            return key == other.key && modifier == keyboard::modifier::none;
        }
        return (key == other.key) && ((modifier & other.modifier) != 0);
    }

    /**
     * @brief compare a key_modifier with a key without modifier
     * @param other the key to compare to
     * @return true if the key_modifier is equal to the key, false otherwise
     */
    constexpr auto operator==(const code &other) const {
        return key == other && modifier == keyboard::modifier::none;
    }

    /**
     * @brief get the string representation of the key_modifier
     * @return the string representation of the key_modifier
     */
    [[nodiscard]] auto string() const {
        auto key_str = key::string(key);
        if(auto mod_str = modifier ::string(modifier); !mod_str.empty()) {
            return mod_str + "+" + key_str;
        }
        return key_str;
    }
};

} // namespace sneze::keyboard
