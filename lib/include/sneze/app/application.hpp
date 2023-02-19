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

#include "../events/events.hpp"
#include "../platform/error.hpp"
#include "../render/render.hpp"

#include "config.hpp"
#include "settings.hpp"
#include "world.hpp"

namespace sneze {

class application {
public:
    inline application(const std::string &team, const std::string &name)
        : team_{team}, name_{name}, settings_{team, name}, render_{std::make_shared<render>()} {}

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

    template<is_value Type>
    [[maybe_unused]] inline void
    get_set_app_setting(const std::string &name, const Type &default_value, Type (*func)(Type)) {
        get_set_setting<Type>(name_, name, default_value, func);
    }

    [[nodiscard]] inline auto world() noexcept -> auto & {
        return world_;
    }

    void app_want_closing(events::application_want_closing event) noexcept;

    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    [[maybe_unused]] void unload_font(const std::string &font_path);

private:
    static const std::int64_t default_width;
    static const std::int64_t default_height;

    std::string team_;
    std::string name_;
    settings settings_;
    sneze::world world_;
    std::shared_ptr<render> render_;
    bool want_to_close_{false};

    auto launch() -> result<>;

    auto read_settings() noexcept -> result<>;

    auto save_settings() noexcept -> result<>;

    [[nodiscard]] auto show_error(const error &err) const -> const auto &;
};

} // namespace sneze
