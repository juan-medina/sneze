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

class application {
public:
    application(const std::string &team, const std::string &name);

    virtual ~application() = default;

    application(const application &) = delete;
    application(const application &&) = delete;

    auto operator=(const application &) -> application & = delete;
    auto operator=(const application &&) -> application & = delete;

    auto run() -> result<bool, error>;

    [[nodiscard]] virtual auto configure() -> config = 0;

    [[nodiscard]] virtual auto init() -> result<> = 0;

    virtual void end() = 0;

    [[nodiscard]] inline auto team() const noexcept -> const std::string & {
        return team_;
    }

    [[nodiscard]] inline auto name() const noexcept -> const std::string & {
        return name_;
    }

    template<is_value Type>
    [[maybe_unused]] inline void set_setting(const std::string &section, const std::string &name, const Type &value) {
        settings_.set<Type>(section, name, value);
    }

    template<is_value Type>
    [[maybe_unused]] [[nodiscard]] inline auto
    get_setting(const std::string &section, const std::string &name, const Type &default_value) {
        return settings_.get<Type>(section, name, default_value);
    }

    template<is_value Type>
    [[maybe_unused]] inline void get_set_setting(const std::string &section,
                                                 const std::string &name,
                                                 const Type &default_value,
                                                 Type (*func)(Type)) {
        Type value = settings_.get(section, name, default_value);
        value = func(value);
        settings_.set<Type>(section, name, value);
    }

    template<is_value Type>
    [[maybe_unused]] inline void set_app_setting(const std::string &name, const Type &value) {
        settings_.set<Type>(name_, name, value);
    }

    template<is_value Type>
    [[maybe_unused]] [[nodiscard]] inline auto get_app_setting(const std::string &name, const Type &default_value) {
        return settings_.get<Type>(name_, name, default_value);
    }

    [[nodiscard]] inline auto world() noexcept -> auto {
        return world_;
    }

    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    [[maybe_unused]] void unload_font(const std::string &font_path);

    [[maybe_unused]] [[nodiscard]] auto load_sprite_sheet(const std::string &sprite_sheet_path) -> result<>;

    [[maybe_unused]] [[nodiscard]] auto load_sprite(const std::string &sprite_path) -> result<>;

    [[maybe_unused]] void unload_sprite_sheet(const std::string &sprite_sheet_path);

    [[maybe_unused]] void unload_sprite(const std::string &sprite_path);

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
