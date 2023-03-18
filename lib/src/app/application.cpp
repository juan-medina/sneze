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
#include "sneze/effects/effects_system.hpp"
#include "sneze/embedded/embedded.hpp"
#include "sneze/events/events.hpp"
#include "sneze/platform/logger.hpp"
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
      world_{std::make_shared<class world>()} {}

auto application::show_error(const error &err) const -> const auto & {
    std::string message = err.get_message();
    auto causes = err.get_causes();
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
    logger::info("running application: {} (Team: {})", name(), team());

    if(auto err = read_settings().ko()) {
        return show_error(*err);
    }

    log_level();

    if(auto err = launch().ko()) {
        return show_error(*err);
    }

    if(auto err = save_settings().ko()) {
        return show_error(*err);
    }

    logger::trace("stopping application: {}", name());

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
    logger::trace("configure application");
    auto config = configure();

    auto [window, fullscreen, monitor] = get_window_settings(config);

    logger::trace("init render");
    if(auto err =
           render_->init(window, config.logical(), fullscreen, monitor, name(), config.icon(), config.clear()).ko()) {
        logger::error("error initializing render");
        return error("Can't init the render system.", *err);
    }

    logger::trace("init world");
    world_->init();

    constexpr auto render_priority = world::priority::after_applications;
    constexpr auto sdl_events_priority = world::priority::before_applications;
    constexpr auto keys_priority = sdl_events_priority - 1;
    constexpr auto layout_priority = render_priority + 1;
    constexpr auto effects_priority = layout_priority + 1;

    logger::trace("adding render system to the world");
    world_->add_system_with_priority_internal<render_priority, render_system>(render_);

    logger::trace("adding event system to the world");
    world_->add_system_with_priority_internal<sdl_events_priority, sdl_events_system>(render_);

    logger::trace("adding key system to the world");
    world_->add_system_with_priority_internal<keys_priority, keys_system>(config.exit(), config.toggle_full_screen());

    logger::trace("adding layout system to the world");
    world_->add_system_with_priority_internal<layout_priority, layout_system>();

    logger::trace("adding effects system to the world");
    world_->add_system_with_priority_internal<effects_priority, effects_system>();

    logger::trace("listening for application_want_closing events");
    world_->add_listener<events::application_want_closing, &application::app_want_closing>(this);

    logger::trace("loading embedded fonts");
    if(auto err = load_embedded_fonts().ko()) {
        logger::error("error loading embedded fonts");
        render_->end();
        return error("Can't init the application.", *err);
    }

    logger::trace("update initial world state");
    world_->update();

    logger::trace("init application");
    if(auto err = init().ko()) {
        logger::error("error initializing application");
        render_->end();
        return error("Can't init the application.", *err);
    }

    while(!want_to_close_) {
        world_->update();
    }

    logger::trace("ending application");
    end();

    logger::trace("unloading embedded fonts");
    unload_embedded_fonts();

    logger::trace("remove any listener by sneze::application");
    world_->remove_listeners(this);

    logger::trace("ending world");
    world_->end();

    logger::trace("save window");
    save_window_settings();

    logger::trace("ending render");
    render_->end();

    return true;
}

auto application::read_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.read().ok(); err) {
        logger::error("error reading settings");
        return error("Can't read settings.", *err); // NOLINT(bugprone-unchecked-optional-access)
    } else {                                        // NOLINT(readability-else-after-return)
        return *val;                                // NOLINT(bugprone-unchecked-optional-access)
    }
}

auto application::save_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.save().ok(); err) {
        logger::error("error saving settings");
        return error("Can't save settings.", *err); // NOLINT(bugprone-unchecked-optional-access)
    } else {                                        // NOLINT(readability-else-after-return)
        return *val;                                // NOLINT(bugprone-unchecked-optional-access)
    }
}

void application::app_want_closing(events::application_want_closing) noexcept { // NOLINT(readability-named-parameter)
    logger::trace("application want to close");
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

auto application::load_sprite_sheet(const std::string &sprite_sheet_path) -> result<> {
    if(auto err = render_->load_sprite_sheet(sprite_sheet_path).ko(); err) {
        logger::error("error loading sprite sheet: {}", sprite_sheet_path);
        return error("Can't load Sprite Sheet.", *err);
    }

    return true;
}

auto application::load_sprite(const std::string &sprite_path) -> result<> {
    if(auto err = render_->load_sprite(sprite_path).ko(); err) {
        logger::error("error loading sprite: {}", sprite_path);
        return error("Can't load Sprite.", *err);
    }

    return true;
}

void application::unload_sprite_sheet(const std::string &sprite_sheet_path) {
    render_->unload_sprite_sheet(sprite_sheet_path);
}

void application::unload_sprite(const std::string &sprite_path) {
    render_->unload_sprite(sprite_path);
}

auto application::get_window_settings(const config &cfg) -> std::tuple<components::size, bool, int> {
    using namespace std::literals;
    auto width = settings_.get("window"s, "width"s, static_cast<std::int64_t>(cfg.window().width));
    auto height = settings_.get("window"s, "height"s, static_cast<std::int64_t>(cfg.window().height));
    auto size = components::size{static_cast<float>(width), static_cast<float>(height)};
    auto monitor = static_cast<int>(settings_.get("window"s, "monitor"s, static_cast<std::int64_t>(0)));

    auto fullscreen = settings_.get("window"s, "fullscreen"s, false);

    return {size, fullscreen, monitor};
}

void application::save_window_settings() {
    using namespace std::literals;
    if(!render_->fullscreen()) {
        auto window = render_->window();
        settings_.set("window"s, "width"s, static_cast<std::int64_t>(window.width));
        settings_.set("window"s, "height"s, static_cast<std::int64_t>(window.height));
    }
    settings_.set("window"s, "fullscreen"s, render_->fullscreen());
    settings_.set("window"s, "monitor"s, static_cast<std::int64_t>(render_->monitor()));
}

auto application::load_embedded_fonts() -> result<> {
    if(auto err = load_font(embedded::mono_font).ko(); err) {
        logger::error("error loading mono font");
        return error("Can't load embedded mono font.", *err);
    }

    if(auto err = load_font(embedded::regular_font).ko(); err) {
        logger::error("error loading regular font");
        return error("Can't load embedded regular font.", *err);
    }

    return true;
}
void application::unload_embedded_fonts() {
    unload_font(embedded::mono_font);
    unload_font(embedded::regular_font);
}

} // namespace sneze
