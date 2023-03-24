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

#include "../components/renderable.hpp"
#include "../device/keyboard.hpp"
#include "../embedded/embedded.hpp"

namespace sneze {

/**
 * @brief Application configuration
 *
 * This class is used to configure the application, it is used by the application::configure() method.
 *
 * All the methods are chainable, so you can do something like this:
 *
 * @code
 * auto my_game::configure() -> config {
 *   return config()
 *            .size(1920, 1080)
 *            .clear(color::light_gray)
 *            .exit(key::escape)
 *            .toggle_full_screen(modifier::alt, key::_return);
 * }
 * @endcode
 *
 * @note The default values of config are:
 * - Window Size: 1920x1080
 * - Logical Size: 1920x1080
 * - Clear color: black
 * - Exit key: NONE
 * - Toggle full screen key: NONE
 * - Icon: sneze icon
 *
 * @see application::configure()
 * @see application::init()
 * @see application::run()
 */
class config {
public:
    /**
     * @brief Construct a new config object
     *
     */
    explicit config() = default;

    ~config() = default;

    config(const config &) = default; // NOLINT(bugprone-exception-escape)
    config(config &&) = default;

    auto operator=(const config &) -> config & = default;
    auto operator=(config &&) -> config & = default;

    /**
     * @brief Set the clear color
     *
     * @param clear The color to use to clear the screen
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto clear(const components::color &clear) -> config {
        clear_ = clear;
        return *this;
    }

    /**
     * @brief Set the exit key, without modifier
     *
     * @param key The key to use to exit the application
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::code &key) -> config {
        exit_ = {key};
        return *this;
    }

    /**
     * @brief Set the exit key and modifier
     *
     * @param modifier The key modifier to use to exit the application
     * @param key The key to use to exit the application
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto exit(const keyboard::mod &modifier, const keyboard::code &key) -> config {
        exit_ = {key, modifier};
        return *this;
    }

    /**
     * @brief Set the toggle full screen key, without modifier
     *
     * @param key The key to use to toggle full screen
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::code &key) -> config {
        toggle_full_screen_ = {key};
        return *this;
    }

    /**
     * @brief Set the toggle full screen key and modifier
     *
     * @param modifier The key modifier to use to toggle full screen
     * @param key The key to use to toggle full screen
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto toggle_full_screen(const keyboard::mod &modifier, const keyboard::code &key)
        -> config {
        toggle_full_screen_ = {key, modifier};
        return *this;
    }

    /**
     * @brief Set the window size
     *
     * Set the initial window size, this is the size of the window in pixels.
     *
     * When the application is closed, the window size is saved, and used as the initial window size when the
     * application is started again and this parameter is ignored.
     *
     * @param width The width of the window
     * @param height The height of the window
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto window(int width, int height) -> config {
        window_.width = static_cast<float>(width);
        window_.height = static_cast<float>(height);
        return *this;
    }

    /** @brief Set the logical size
     *
     * Set the logical size, this is the size of the window in game units.
     *
     * When the window is resized, the logical size is used to calculate the scale factor, and the game is scaled,
     * however the game area will expand to fill the window and maintaining the aspect ratio, making that we could end
     * with a logical size that is bigger or smaller than the original logical size in one of the axis.
     *
     * @param width The width of the logical size
     * @param height The height of the logical size
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto logical(int width, int height) -> config {
        logical_.width = static_cast<float>(width);
        logical_.height = static_cast<float>(height);
        return *this;
    }

    /**
     * @brief Set the window size and logical size
     *
     * @param width The width of the window and logical size
     * @param height The height of the window and logical size
     * @return config& A reference to the config object to allow chaining
     * @see config::window()
     * @see config::logical()
     */
    [[maybe_unused]] [[nodiscard]] auto size(int width, int height) -> config {
        return window(width, height).logical(width, height);
    }

    /**
     * @brief Set the window icon
     *
     * @param icon The path to the icon file
     * @return config& A reference to the config object to allow chaining
     */
    [[maybe_unused]] [[nodiscard]] auto icon(const std::string &icon) -> config {
        icon_ = icon;
        return *this;
    }

    /** @brief get the clear color
     *
     * @return the clear color
     */
    [[nodiscard]] inline auto get_clear_color() const -> const auto & {
        return clear_;
    }

    /** @brief Get the exit key
     *
     * @return const auto& The exit key
     */
    [[nodiscard]] inline auto get_exit_key() const -> const auto & {
        return exit_;
    }

    /** @brief Get the toggle full screen key
     *
     * @return const auto& The toggle full screen key
     */
    [[nodiscard]] inline auto get_toggle_full_screen_key() const -> const auto & {
        return toggle_full_screen_;
    }

    /** @brief Get the window size
     *
     * @return const auto& The window size
     */
    [[nodiscard]] inline auto get_window_size() const -> const auto & {
        return window_;
    }

    /** @brief Get the logical size
     *
     * @return const auto& The logical size
     */
    [[nodiscard]] inline auto get_logical_size() const -> const auto & {
        return logical_;
    }

    /** @brief Get the window icon
     *
     * @return const auto& The window icon
     */
    [[nodiscard]] inline auto get_window_icon() const -> const auto & {
        return icon_;
    }

private:
    //! The window size
    components::size window_ = {1920, 1080};

    //! The logical size
    components::size logical_ = {1920, 1080};

    //! The clear color
    components::color clear_ = components::color::black;

    //! The exit key
    keyboard::key_modifier exit_ = {keyboard::key::unknown, keyboard::modifier::none};

    //! The toggle full screen key
    keyboard::key_modifier toggle_full_screen_ = {keyboard::key::unknown, keyboard::modifier::none};

    //! The window icon
    std::string icon_ = embedded::sneze_logo;
};

} // namespace sneze
