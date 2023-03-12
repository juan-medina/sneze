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
#include "sneze/app/world.hpp"

#include "sneze/platform/logger.hpp"

#include <chrono>

namespace sneze {

void world::update() {
    update_time();
    update_systems();
    sent_events();
}

void world::update_systems() {
    if(!systems_to_add_.empty()) {
        add_pending_systems();
    }

    if(!systems_to_remove_.empty()) {
        remove_pending_systems();
    }

    for(auto &system: systems_) {
        system->update(this);
    }
}

void world::add_pending_systems() {
    logger::trace("adding systems");
    for(auto &system: systems_to_add_) {
        systems_.push_back(std::move(system));
        systems_.back()->init(this);
    }
    systems_to_add_.clear();
    logger::trace("sorting systems");
    std::sort(systems_.begin(), systems_.end(), world::sort_by_priority);
}

void world::remove_pending_systems() noexcept {
    logger::trace("checking if we need to delete any system");
    for(auto type_to_remove: systems_to_remove_) {
        if(!remove_system_from_vector(type_to_remove, systems_)) {
            remove_system_from_vector(type_to_remove, systems_to_add_);
        }
    }
    systems_to_remove_.clear();
}

auto world::remove_system_from_vector(entt::id_type type_to_remove, systems_vector &systems) noexcept -> bool {
    auto begin = systems.begin();
    auto end = systems.end();
    auto matcher = world::match_type(type_to_remove);
    if(auto it_system = std::find_if(begin, end, matcher); it_system != end) [[likely]] {
        it_system->get()->end(this);
        systems.erase(it_system);
        return true;
    }

    return false;
}

void world::remove_all_systems() noexcept {
    remove_all_systems_from_vector(systems_);
    remove_all_systems_from_vector(systems_to_add_);
}

void world::remove_all_systems_from_vector(systems_vector &systems) noexcept {
    for(auto &system: systems) {
        system->end(this);
    }
    systems.clear();
}

auto world::sort_by_priority(const system_ptr &lhs, const system_ptr &rhs) noexcept -> bool {
    return lhs->priority() > rhs->priority();
}

void world::sent_events() {
    event_dispatcher_.update();
}

void world::discard_pending_events() noexcept {
    event_dispatcher_.clear();
}

void world::clear() {
    logger::trace("world clean up");

    logger::trace("discarding pending events");
    discard_pending_events();

    logger::trace("removing systems pending to be deleted");
    remove_pending_systems();

    logger::trace("removing all systems");
    remove_all_systems();

    logger::trace("resetting dispatcher");
    event_dispatcher_ = entt::dispatcher{};

    logger::trace("removing globals");
    remove_global<game_time>();

    logger::trace("checking for not clear entities & orphans");
    registry_.each([this](entt::entity entity) {
        if(registry_.orphan(entity)) {
            logger::warning("orphan entity found: {}", static_cast<ENTT_ID_TYPE>(entity));
            registry_.release(entity);
        } else {
            logger::trace("entity not cleared found: {}", static_cast<ENTT_ID_TYPE>(entity));
        }

        for(auto storage: registry_.storage()) {
            if(storage.second.contains(entity)) {
                auto storage_type = storage.second.type();
                auto name = storage_type.name();
                logger::trace("   contains component: {}", name);
            }
        }
    });

    logger::trace("clear registry");
    registry_.clear();
}

auto world::since_epoch() -> float {
    namespace chrono = std::chrono;
    auto milliseconds =
        std::chrono::duration<double, std::milli>(chrono::steady_clock::now().time_since_epoch()).count();
    return static_cast<float>(milliseconds);
}

void world::init() {
    logger::trace("world init");
    start_.elapsed = since_epoch();
    start_.delta = 0.0;
}

void world::end() {
    logger::trace("world end");
    clear();
}

void world::update_time() {
    auto last = current_;
    current_.elapsed = since_epoch() - start_.elapsed;
    current_.delta = current_.elapsed - last.elapsed;
    set_global<game_time>(current_);
}

} // namespace sneze
