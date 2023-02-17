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

#include "sneze/events/events.hpp"
#include "sneze/platform/logger.hpp"
#include "sneze/platform/version.hpp"
#include "sneze/systems/render_system.hpp"

#include <boxer/boxer.h>
#include <fmt/format.h>

namespace sneze {

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
    logger::setup_log();

    logger::info("{}", version::string);
    logger::debug("running application: {} (Team: {})", name(), team());

    if(auto err = read_settings().ko()) return show_error(*err);
    if(auto err = launch().ko()) return show_error(*err);
    if(auto err = save_settings().ko()) return show_error(*err);

    logger::debug("stopping application: {}", name());

    return true;
}

auto application::launch() -> result<> {
    auto width = settings_.get("window", "width", default_width);
    auto height = settings_.get("window", "height", default_height);

    logger::debug("init application");
    auto setup = init();

    logger::debug("init render");
    if(auto err = render_->init(width, height, name(), setup.clear_color()).ko()) {
        logger::error("error initializing render");
        return error("Can't init the render system.", *err);
    }

    logger::debug("adding render system to the world");
    world_.add_system_with_priority<render_system>(world::priority::lowest, render_);

    logger::debug("listening for application_want_closing events");
    world_.add_listener<events::application_want_closing, &application::app_want_closing>(this);

    while(!want_to_close_) world_.update();

    logger::debug("ending application");
    end();

    logger::debug("remove any listener by sneze::application");
    world_.remove_listeners(this);

    logger::debug("clear world");
    world_.clear();

    logger::debug("ending render");
    render_->end();

    return true;
}

auto application::read_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.read().ok(); err) {
        logger::error("error reading settings");
        return error("Can't read settings.", *err);
    } else {
        return *val;
    }
}

auto application::save_settings() noexcept -> result<> {
    if(auto [val, err] = settings_.save().ok(); err) {
        logger::error("error saving settings");
        return error("Can't save settings.", *err);
    } else {
        return *val;
    }
}

void application::app_want_closing(events::application_want_closing) noexcept {
    logger::debug("application want to close");
    want_to_close_ = true;
}

const std::int64_t application::default_width = {1920LL};
const std::int64_t application::default_height = {1080LL};

} // namespace sneze
