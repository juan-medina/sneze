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

#include <memory>
#include <string>

#include "../components/geometry.hpp"
#include "../events/events.hpp"
#include "../platform/error.hpp"

#include "config.hpp"
#include "settings.hpp"

namespace sneze {

class render;
class world;

/**
 * @brief The class defines an sneze application and controls the world, render, settings, events and systems.
 *
 * @note You MUST inherit from this class and implement the following methods:
 * application::configure, application::init and application::end.
 */
class application {
public:
    /**
     * @brief construct a new application
     *
     * @note You need to call this constructor from your own application constructor setting the team and name of
     * your game these values will be used to create the window title and the settings folder and file name.
     *
     * @code
     * class my_game: public application {
     * public:
     *   my_game(): application("Team Name", "My Game") {}
     *   ...
     * };
     * @endcode
     * @param team the team name for your game
     * @param name the name of your game
     */
    application(const std::string &team, const std::string &name);

    virtual ~application() = default;

    application(const application &) = delete;
    application(const application &&) = delete;

    auto operator=(const application &) -> application & = delete;
    auto operator=(const application &&) -> application & = delete;

    /**
     * @brief configure the application
     *
     * This method is called before the application::init method and it is used to configure the application.
     * You can set several options like the window size, the window title, the window icon, and many others, for more
     * information about the available options please check the sneze::config class.
     *
     * @code
     * my_game::configure() {
     *   return config()
     *     .size(1920, 1080)
     *     .clear(color::light_gray)
     *     .exit(key::escape)
     *     .toggle_full_screen(modifier::alt, key::_return);
     * }
     * @endcode
     * @return the desired application configuration
     * @see sneze::config
     */
    [[nodiscard]] virtual auto configure() -> config = 0;

    /**
     * @brief notify the application about initialization
     * This method is called after the application::configure method and it is used to initialize the application.
     *
     * You can load the resources you need for your game here, like fonts, sprites, and others.
     *
     * @return a result with the error if any
     * @see sneze::result
     */
    [[nodiscard]] virtual auto init() -> result<> = 0;

    /**
     * @brief notify the application about ending
     * This method is called before the application closes and it is used to end the application.
     *
     * You can unload the resources you loaded in the application::init method here.
     *
     */
    virtual void end() = 0;

    /**
     * @brief ask the application to run
     *
     * tell the application to start running, this method will call the application::configure, application::init and
     * will start the main loop, after the main loop ends the application::end method will be called.
     *
     * @code
     * auto main(int argc, char * argv[]) -> int {
     *   if(auto err = my_game().run().ko(); err) {
     *     return EXIT_FAILURE;
     *   }
     *   return EXIT_SUCCESS;
     * }
     * @endcode
     *
     * @return a result with the error if any
     * @see sneze::result
     * @see sneze::result::ko
     * @see sneze::error
     * @see sneze::application::configure
     * @see sneze::application::init
     * @see sneze::application::end
     */
    [[nodiscard]] auto run() -> result<bool, error>;

    //! get the team name for your game
    [[nodiscard]] inline auto get_team() const noexcept -> const std::string & {
        return team_;
    }

    //! get the game name
    [[nodiscard]] inline auto get_name() const noexcept -> const std::string & {
        return name_;
    }

    /**
     * @brief Sets an section+name setting to the given value.
     *
     * This method is used to set an setting, the setting will be saved in the settings file.
     *
     * The settings file is located in the user's configuration folder, this folder is different for each platform:
     *
     * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
     * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
     * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
     *
     * @note If you want to set a setting for the application, you should use application::set_app_setting instead.
     *
     * @code
     * auto my_game::init() -> result<> {
     *   set_setting("section_name", "setting_name", "value");
     *   return true;
     * }
     * @endcode
     *
     * This code will set in the settings file the following setting:
     *
     * @code{.cpp}
     * {
     *   "section_name": {
     *     "setting_name": "value"
     *   },
     * }
     * @endcode
     *
     * @tparam Type the type of the setting value. The type must be one of: bool, std::int64_t, double or std::string
     * @param section the section name
     * @param name the setting name
     * @param value the setting value
     * @see sneze::application::get_setting
     * @see sneze::application::set_app_setting
     * @see sneze::settings::set
     */
    template<is_value Type>
    [[maybe_unused]] inline void set_setting(const std::string &section, const std::string &name, const Type &value) {
        settings_.set<Type>(section, name, value);
    }

    /**
     * @brief Get the value from an given section+name.
     *
     * This method is used to get an setting value.
     *
     * The settings file is located in the user's configuration folder, this folder is different for each platform:
     *
     * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
     * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
     * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
     *
     * @note If you want to get a setting for the application, you should use application::get_app_setting instead.
     *
     * @code
     * auto my_game::init() -> result<> {
     *   auto value = get_setting("section_name", "setting_name", "default_value");
     *   return true;
     * }
     * @endcode
     *
     * @tparam Type the type of the setting value. The type must be one of: bool, std::int64_t, double or std::string
     * @param section the section name
     * @param name the setting name
     * @param default_value the default value to return if the setting is not found
     * @return the setting value or the default value if the setting is not found
     * @see sneze::application::set_setting
     * @see sneze::application::get_app_setting
     * @see sneze::settings::get
     */
    template<is_value Type>
    [[maybe_unused]] [[nodiscard]] inline auto
    get_setting(const std::string &section, const std::string &name, const Type &default_value) {
        return settings_.get<Type>(section, name, default_value);
    }

    /**
     * @brief Sets an application setting given the name and value.
     *
     * This method is used to set a setting, the setting will be saved in the settings file.
     *
     * The settings file is located in the user's configuration folder, this folder is different for each platform:
     *
     * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
     * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
     * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
     *
     * @code
     * auto my_game::init() -> result<> {
     *   set_app_setting("setting_name", "value");
     *   return true;
     * }
     * @endcode
     *
     * This code will set in the settings file the following setting:
     *
     * @code{.cpp}
     * {
     *   "My Game": {
     *     "setting_name": "value"
     *   },
     * }
     * @endcode
     *
     * @tparam Type the type of the setting value. The type must be one of: bool, std::int64_t, double or std::string
     * @param name the setting name
     * @param value the setting value
     * @see sneze::application::get_app_setting
     * @see sneze::application::set_setting
     */
    template<is_value Type>
    [[maybe_unused]] inline void set_app_setting(const std::string &name, const Type &value) {
        settings_.set<Type>(name_, name, value);
    }

    /**
     * @brief Get the value from an given name.
     *
     * This method is used to get an setting value.
     *
     * The settings file is located in the user's configuration folder, this folder is different for each platform:
     *
     * - Windows: %APPDATA%\\sneze\\[team]\\[name]\\settings.json
     * - Linux: $XDG_CONFIG_HOME/sneze/[team]/[name]/settings.json
     * - macOS: $HOME/Library/Application Support/sneze/[team]/[name]/settings.json
     *
     * @code
     * auto my_game::init() -> result<> {
     *   auto value = get_app_setting("setting_name", "default_value");
     *   return true;
     * }
     * @endcode
     *
     * @tparam Type the type of the setting value. The type must be one of: bool, std::int64_t, double or std::string
     * @param name the setting name
     * @param default_value the default value to return if the setting is not found
     * @return the setting value or the default value if the setting is not found
     * @see sneze::application::set_app_setting
     * @see sneze::application::get_setting
     * @see sneze::settings::get
     */
    template<is_value Type>
    [[maybe_unused]] [[nodiscard]] inline auto get_app_setting(const std::string &name, const Type &default_value) {
        return settings_.get<Type>(name_, name, default_value);
    }

    /**
     * @brief Get the ECS world.
     * @return the ECS world
     * @see sneze::world
     */
    [[nodiscard]] inline auto world() noexcept -> auto {
        return world_;
    }

    /**
     * @brief Load a BITMAP font from a given path.
     *
     * This method is used to load a BITMAP font from a given path, the font will be loaded only once,
     * if the font is already loaded, the method will return true.
     *
     * @note You should unload the font when you don't need it anymore using the unload_font method.
     *
     * @code
     * my_game::init() -> result<> {
     *   if(auto err = load_font("fonts/my_font.fnt").ko()) {
     *     logger::error("game can't load font");
     *     return error("Can't load game font.", *err);
     *   }
     *   return true;
     * }
     * @endcode
     *
     * @param font_path the path to the font
     *
     * @return if the font was loaded successfully
     * @see sneze::application::unload_font
     */
    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    /**
     * @brief Unload a BITMAP font from a given path.
     *
     * This method is used to unload a BITMAP font from a given path, the font will be unloaded if there are no more
     * references to it.
     *
     * @note You should load the font before using it using the application::load_font, however this method will
     * not fail if the font is not loaded.
     *
     * @code
     * my_game::end() {
     *   unload_font("fonts/my_font.fnt");
     * }
     * @endcode
     *
     * @param font_path the path to the font
     *
     * @see sneze::application::load_font
     */
    [[maybe_unused]] void unload_font(const std::string &font_path);

    /**
     * @brief Load a sprite font from a given path.
     *
     * This method is used to load a sprite for a given path, the sprite will be loaded only once,
     * if the sprite is already loaded, the method will return true.
     *
     * @note You should unload the sprite when you don't need it anymore using the unload sprite method.
     *
     * @code
     * my_game::init() -> result<> {
     *   if(auto err = load_sprite("sprites/my_sprite.png").ko()) {
     *     logger::error("game can't load sprite");
     *     return error("Can't load sprite.", *err);
     *   }
     *   return true;
     * }
     * @endcode
     *
     * @param sprite_path the path to the sprite
     *
     * @return if the sprite was loaded successfully
     * @see sneze::application::unload_sprite
     */
    [[maybe_unused]] [[nodiscard]] auto load_sprite(const std::string &sprite_path) -> result<>;

    /**
     * @brief Unload a sprite from a given path.
     *
     * This method is used to unload a sprite from a given path, the sprite be unloaded if there are no more
     * references to it.
     *
     * @note You should load the sprite before using it using the application::load_sprite, however this method will
     * not fail if the sprite is not loaded.
     *
     * @code
     * my_game::end() {
     *   unload_sprite("sprites/my_sprite.png");
     * }
     * @endcode
     *
     * @param sprite_path the path to the sprite
     *
     * @see sneze::application::load_sprite
     */
    [[maybe_unused]] void unload_sprite(const std::string &sprite_path);

    [[maybe_unused]] [[nodiscard]] auto load_sprite_sheet(const std::string &sprite_sheet_path) -> result<>;

    [[maybe_unused]] void unload_sprite_sheet(const std::string &sprite_sheet_path);

private:
    std::string team_;
    std::string name_;
    settings settings_;
    std::shared_ptr<render> render_;
    std::shared_ptr<class world> world_;
    bool want_to_close_{false};



    void app_want_closing(events::application_want_closing event) noexcept;

    auto launch() -> result<>;

    auto read_settings() noexcept -> result<>;

    auto save_settings() noexcept -> result<>;

    void log_level();

    [[nodiscard]] auto show_error(const error &err) const -> const auto &;

    void save_window_settings();

    [[nodiscard]] auto get_window_settings(const config &cfg) -> std::tuple<components::size, bool, int>;

    [[nodiscard]] auto load_embedded_fonts() -> result<>;

    void unload_embedded_fonts();
};

} // namespace sneze
