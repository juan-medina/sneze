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

#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wmissing-braces"
#endif
#include <entt/entt.hpp>
#if defined(__clang__)
#    pragma clang diagnostic pop
#endif

#include "../components/generic.hpp"
#include "../events/events.hpp"
#include "../globals/globals.hpp"
#include "../systems/system.hpp"

namespace sneze {

class world {
    friend class application;

public:
    world() = default;
    ~world() = default;

    world(const world &) = delete;
    world(const world &&) = delete;

    auto operator=(const world &) -> world & = delete;
    auto operator=(const world &&) -> world & = delete;

    template<typename... Args>
    auto add_entity(Args... args) {
        auto entity_id = registry_.create();
        recurse_create(entity_id, args...);
        return entity_id;
    }

    [[maybe_unused]] void remove_entity(entt::entity entity) {
        registry_.destroy(entity);
    }

    template<typename Type>
    [[maybe_unused]] auto get_component(entt::entity entity) -> decltype(auto) {
        return registry_.get<Type>(entity);
    }

    template<typename Type>
    [[maybe_unused]] auto remove_component(entt::entity entity) {
        registry_.remove<Type>(entity);
    }

    template<typename Type, typename... Args>
    void set_component(entt::entity entity, Args &&...args) {
        registry_.emplace<Type>(entity, std::forward<Args>(args)...);
    }

    template<typename Type>
    auto has_component(entt::entity entity) {
        return registry_.try_get<Type>(entity);
    }

    template<typename... Types>
    [[nodiscard]] auto get_entities() {
        return registry_.view<Types...>().each();
    }

    template<typename TagType>
    void tag(entt::entity entity) {
        set_component<components::tag<TagType>>(entity);
    }

    template<typename TagType>
    void untag(entt::entity entity) {
        remove_component<components::tag<TagType>>(entity);
    }

    template<typename TagType, typename... Types>
    [[nodiscard]] auto get_tagged() {
        return registry_.view<Types..., components::tag<TagType>>().each();
    }

    template<typename TagType>
    void remove_all_tagged() {
        for(auto &&[entity]: get_tagged<TagType>()) {
            remove_entity(entity);
        }
    }

    template<typename TagType>
    void remove_all_tags() {
        for(auto &&[entity]: get_tagged<TagType>()) {
            untag<TagType>(entity);
        }
    }

    template<typename Type, typename... Args>
    [[maybe_unused]] void set_global(Args &&...args) {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        auto &&global = get_global<Type>();
        global = Type{std::forward<Args>(args)...};
    }

    template<typename Type>
    [[maybe_unused]] [[nodiscard]] auto get_global() -> auto & {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        if(auto first = registry_.view<Type, components::tag<global<Type>>>().front(); !(first == entt::null)) {
            return get_component<Type>(first);
        }
        tag<global<Type>>(add_entity(Type{}));
        return get_global<Type>();
    }

    template<typename Type>
    [[maybe_unused]] void remove_global() {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        remove_all_tagged<global<Type>>();
    }

    template<typename Type, typename Compare>
    void sort(Compare compare) {
        registry_.sort<Type>(compare);
    }

    template<typename SystemType, typename... Args>
    [[maybe_unused]] void add_system(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        add_system_with_priority<priority::normal, SystemType>(args...);
    }

    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        static_assert(Priority >= world::priority::low && Priority <= world::priority::high,
                      "priority must be between priority::low (-1000) and priority::high (1000)");
        add_system_with_priority_internal<Priority, SystemType>(args...);
    }

    template<typename SystemType>
    [[maybe_unused]] void remove_system() noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        auto constexpr type_to_remove = entt::type_hash<SystemType>::value();
        systems_to_remove_.push_back(type_to_remove);
    }

    template<typename EventType, auto Candidate, typename InstanceType>
    void add_listener(InstanceType &&instance) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.sink<EventType>().template connect<Candidate>(instance);
    }

    template<typename EventType, auto Candidate, typename InstanceType>
    [[maybe_unused]] void remove_listener(InstanceType &&instance) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.sink<EventType>().template disconnect<Candidate>(instance);
    }

    template<typename EventType, typename InstanceType>
    [[maybe_unused]] void remove_listener(InstanceType &&instance) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.sink<EventType>().disconnect(instance);
    }

    template<typename InstanceType>
    [[maybe_unused]] void remove_listeners(InstanceType &&instance) {
        event_dispatcher_.disconnect(instance);
    }

    template<typename EventType, typename... Args>
    void emmit(Args &&...args) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.enqueue<EventType>(this, std::forward<Args>(args)...);
    }

    template<typename ComponentType, auto Candidate, typename InstanceType>
    void add_listener_to_add_component(InstanceType &&instance) {
        registry_.on_construct<ComponentType>()
            .template connect<&world::add_listener_to_add_component_internal<ComponentType>>(this);
        add_listener<events::add_component<ComponentType>, Candidate>(instance);
    }

    template<typename ComponentType, typename InstanceType>
    void remove_listener_to_add_component(InstanceType &&instance) {
        registry_.on_construct<ComponentType>()
            .template disconnect<&world::add_listener_to_add_component_internal<ComponentType>>(this);
        remove_listener<events::add_component<ComponentType>>(instance);
    }

    template<typename ComponentType, typename InstanceType>
    [[maybe_unused]] void remove_component_listeners(InstanceType &&instance) {
        remove_listener_to_add_component<ComponentType>(instance);
    }

protected:
    void init();

    void update();

    void end();

    void clear();

    enum priority : int32_t {
        low = -1000,
        normal = 0,
        high = 1000,

        before_applications = high + 1000,
        after_applications = low - 1000,
    };

    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority_internal(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        static_assert(Priority >= world::priority::after_applications
                          && Priority <= world::priority::before_applications,
                      "priority must be between priority::after_applications and priority::before_applications");
        add_system_with_priority_unrestricted<Priority, SystemType>(args...);
    }

private:
    [[nodiscard]] static auto since_epoch() -> float;

    template<typename ComponentType>
    void add_listener_to_add_component_internal(entt::registry & /*registry*/, entt::entity entity) {
        auto &component = registry_.get<ComponentType>(entity);
        emmit<events::add_component<ComponentType>>(entity, component);
    }

    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority_unrestricted(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        auto constexpr type_hash = entt::type_hash<SystemType>::value();
        systems_to_add_.push_back(
            std::make_unique<system_with_priority>(type_hash, Priority, std::make_unique<SystemType>(args...)));
    }

    template<typename Type>
    struct global {};

    using system_ptr = std::unique_ptr<system_with_priority>;
    using systems_vector = std::vector<system_ptr>;
    using systems_id_vector = std::vector<entt::id_type>;

    game_time start_;
    game_time current_;

    systems_vector systems_;
    systems_vector systems_to_add_;

    systems_id_vector systems_to_remove_;
    entt::registry registry_;

    entt::dispatcher event_dispatcher_;

    void update_time();

    void update_systems();

    void sent_events();

    void remove_all_systems() noexcept;

    void discard_pending_events() noexcept;

    void remove_pending_systems() noexcept;

    static auto match_type(entt::id_type type_to_remove) noexcept {
        return [type_to_remove](const system_ptr &system) noexcept -> bool { return system->type() == type_to_remove; };
    }

    void add_pending_systems();

    auto remove_system_from_vector(entt::id_type type_to_remove, systems_vector &systems) noexcept -> bool;

    void remove_all_systems_from_vector(systems_vector &systems) noexcept;

    template<typename... Args>
    void recurse_create(entt::entity entity_id, Args... args) {
        helper_create_shift(entity_id, args...);
    }

    template<typename Type, typename... Args>
    [[maybe_unused]] void helper_create_shift(entt::entity entity_id, Type value, Args &&...args) {
        set_component<Type>(entity_id, value);
        recurse_create(entity_id, args...);
    }

    void helper_create_shift(entt::entity) {} // NOLINT(readability-named-parameter)

    static auto sort_by_priority(const system_ptr &lhs, const system_ptr &rhs) noexcept -> bool;
};

} // namespace sneze
