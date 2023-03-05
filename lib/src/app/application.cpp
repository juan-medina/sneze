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

#include "sneze/app/application.hpp"

#include "sneze/app/world.hpp"
#include "sneze/events/events.hpp"
#include "sneze/platform/logger.hpp"
#include "sneze/platform/version.hpp"
#include "sneze/render/render.hpp"
#include "sneze/systems/keys_system.hpp"
#include "sneze/systems/layout_system.hpp"
#include "sneze/systems/render_system.hpp"
#include "sneze/systems/sdl_events_system.hpp"

#include <string>

#include <boxer/boxer.h>
#include <fmt/format.h>

namespace sneze {

application::application(const std::string &team, const std::string &name)
    : team_{team}, name_{name}, settings_{team, name}, render_{std::make_shared<render>()},
      world_{std::make_shared<sneze::world>()} {}

auto application::show_error(const error &err) const -> const auto & {
    std::string message = err.message();
    auto causes = err.causes();
    if(!causes.empty()) {
        message += " Caused by:\n";
        for(const auto &cause: causes) {
            message += "\n - " + cause;
        }
    }
    auto message_title = fmt::format("{} : Error!", name());
    boxer::show(message.c_str(), message_title.c_str(), boxer::Style::Error, boxer::Buttons::Quit);
    return err;
}

auto application::run() -> result<bool, error> {
#if defined(NDEBUG) && defined(_WIN32)
#    pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
    logger::setup_log();

    logger::info("{}", version::string);
    logger::debug("running application: {} (Team: {})", name(), team());

    if(auto err = read_settings().ko()) return show_error(*err);

    log_level();

    if(auto err = launch().ko()) return show_error(*err);

    if(auto err = save_settings().ko()) return show_error(*err);

    logger::info("stopping application: {}", name());

    return true;
}

void application::log_level() {
    using namespace std::literals;
    auto const section = "log"s;
#ifdef NDEBUG
    auto const initial_level = "error"s;
    auto const setting = "level"s;
#else
    auto const initial_level = "debug"s;
    auto const setting = "level_debug"s;
#endif
    auto log_level = settings_.get(section, setting, initial_level);

    auto level = logger::level_from_string(log_level);
    auto level_str = logger::string_from_level(level);
    settings_.set(section, setting, level_str);

    logger::info("switching log level to: {}", level_str);

    logger::set_level(level);
}

auto application::launch() -> result<> {
    logger::debug("configure application");
    auto config = configure();

    auto [size, fullscreen, monitor] = get_window_settings(config);

    logger::debug("init render");
    if(auto err = render_->init(size, fullscreen, monitor, name(), config.clear()).ko()) {
        logger::error("error initializing render");
        return error("Can't init the render system.", *err);
    }

    logger::debug("init world");
    world_->init();

    constexpr auto render_priority = world::priority::after_applications;
    constexpr auto sdl_events_priority = world::priority::before_applications;
    constexpr auto keys_priority = sdl_events_priority - 1;
    constexpr auto layout_priority = render_priority + 1;

    logger::debug("adding render system to the world");
    world_->add_system_with_priority_internal<render_priority, render_system>(render_);

    logger::debug("adding event system to the world");
    world_->add_system_with_priority_internal<sdl_events_priority, sdl_events_system>(render_);

    logger::debug("adding key system to the world");
    world_->add_system_with_priority_internal<keys_priority, keys_system>(config.exit(), config.toggle_full_screen());

    logger::debug("adding layout system to the world");
    world_->add_system_with_priority_internal<layout_priority, layout_system>();

    logger::debug("listening for application_want_closing events");
    world_->add_listener<events::application_want_closing, &application::app_want_closing>(this);

    logger::debug("update initial world state");
    world_->update();

    logger::debug("init application");
    if(auto err = init().ko()) {
        logger::error("error initializing application");
        render_->end();
        return error("Can't init the application.", *err);
    }

    while(!want_to_close_) world_->update();

    logger::debug("ending application");
    end();

    logger::debug("remove any listener by sneze::application");
    world_->remove_listeners(this);

    logger::debug("ending world");
    world_->end();

    logger::debug("save window");
    save_window_settings();

    logger::debug("ending render");
    render_->end();

    return true;
}

auto application::read_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.read().ok(); err) {
        logger::error("error reading settings");
        return error("Can't read settings.", *err); // NOLINT(bugprone-unchecked-optional-access)
    } else {
        return *val; // NOLINT(bugprone-unchecked-optional-access)
    }
}

auto application::save_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.save().ok(); err) {
        logger::error("error saving settings");
        return error("Can't save settings.", *err); // NOLINT(bugprone-unchecked-optional-access)
    } else {
        return *val; // NOLINT(bugprone-unchecked-optional-access)
    }
}

void application::app_want_closing(events::application_want_closing) noexcept {
    logger::debug("application want to close");
    want_to_close_ = true;
}

auto application::load_font(const std::string &font_path) -> result<> {
    if(auto err = render_->load_font(font_path).ko(); err) {
        logger::error("error loading font: {}", font_path);
        return error("Can't load font.", *err);
    }

    return true;
}

void application::unload_font(const std::string &font_path) {
    render_->unload_font(font_path);
}

auto application::get_window_settings(const config &cfg) -> const std::tuple<components::size, bool, int> {
    using namespace std::literals;
    auto width = settings_.get("window"s, "width"s, static_cast<std::int64_t>(cfg.size().width));
    auto height = settings_.get("window"s, "height"s, static_cast<std::int64_t>(cfg.size().height));
    auto size = components::size{static_cast<float>(width), static_cast<float>(height)};
    auto monitor = static_cast<int>(settings_.get("window"s, "monitor"s, static_cast<std::int64_t>(0)));

    auto fullscreen = settings_.get("window"s, "fullscreen"s, false);

    return {size, fullscreen, monitor};
}

void application::save_window_settings() {
    using namespace std::literals;
    if(!render_->fullscreen()) {
        auto size = render_->size();
        settings_.set("window"s, "width"s, static_cast<std::int64_t>(size.width));
        settings_.set("window"s, "height"s, static_cast<std::int64_t>(size.height));
    }
    settings_.set("window"s, "fullscreen"s, render_->fullscreen());
    settings_.set("window"s, "monitor"s, static_cast<std::int64_t>(render_->monitor()));
}

} // namespace sneze
