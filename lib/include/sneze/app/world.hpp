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

#include <any>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include <entt/entt.hpp>

#include "../systems/system.hpp"

template<typename Type>
concept has_trivial_constructor = std::is_trivially_constructible<Type>::value;

template<typename Type, typename Interface>
concept implements_interface = std::is_base_of<Interface, Type>::value;

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

    void init();

    void end();

    [[nodiscard]] auto since_epoch() const;

    template<typename Type, typename... Args>
    void set(entt::entity entity, Args &&...args) {
        registry_.emplace<Type>(entity, std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto create(Args... args) {
        auto id = registry_.create();
        recurse_create(id, args...);
        return id;
    }

    template<typename Type>
    auto get(entt::entity entity) {
        registry_.get<Type>(entity);
    }

    template<typename Type>
    auto has(entt::entity entity) {
        if(auto ptr = registry_.try_get<Type>(entity)) {
            return std::optional<Type>{*ptr};
        } else {
            return std::optional<Type>(std::nullopt);
        }
    }

    template<typename... Types>
    [[nodiscard]] auto view() {
        return registry_.view<Types...>().each();
    }

    template<typename Type, typename Compare>
    void sort(Compare compare) {
        registry_.sort<Type>(compare);
    }

    template<typename EventType, auto Candidate, typename InstanceType>
    void add_listener(InstanceType &&instance) {
        event_dispatcher_.sink<EventType>().template connect<Candidate>(instance);
    }

    template<typename EventType, auto Candidate, typename InstanceType>
    [[maybe_unused]] void remove_listener(InstanceType &&instance) {
        event_dispatcher_.sink<EventType>().template disconnect<Candidate>(instance);
    }

    template<typename InstanceType>
    [[maybe_unused]] void remove_listeners(InstanceType &&instance) {
        event_dispatcher_.disconnect(instance);
    }

    template<typename EventType>
    void emmit(EventType event) {
        event_dispatcher_.enqueue(event);
    }

    enum priority : int32_t {
        highest = std::numeric_limits<int32_t>::max(),
        high = highest >> 1,

        lowest = std::numeric_limits<int32_t>::min(),
        low = lowest >> 1,

        normal = 0,
    };

    template<implements_interface<system> SystemType, typename... Args>
    [[maybe_unused]] void add_system(Args... args) noexcept {
        add_system_with_priority<SystemType>(priority::normal, args...);
    }

    template<implements_interface<system> SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority(int32_t priority, Args... args) noexcept {
        auto constexpr type_hash = entt::type_hash<SystemType>::value();
        systems_to_add_.push_back(
            std::make_unique<system_with_priority>(type_hash, priority, std::make_unique<SystemType>(args...)));
    }

    template<implements_interface<system> SystemType>
    [[maybe_unused]] void remove_system() noexcept {
        auto constexpr type_to_remove = entt::type_hash<SystemType>::value();
        systems_to_remove_.push_back(type_to_remove);
    }

    template<has_trivial_constructor Type>
    [[maybe_unused]] [[nodiscard]] auto global() -> const auto {
        auto constexpr type_hash = entt::type_hash<Type>::value();
        if(const auto search = globals_.find(type_hash); search != globals_.end()) {
            return std::any_cast<Type>(search->second);
        } else {
            return Type{};
        }
    }

    template<has_trivial_constructor Type>
    [[maybe_unused]] void global(const Type &value) {
        auto constexpr type_hash = entt::type_hash<Type>::value();
        globals_[type_hash] = value;
    }

    [[nodiscard]] inline auto elapsed() const -> auto & {
        return elapsed_;
    }

    [[nodiscard]] inline auto delta() const -> auto & {
        return delta_;
    }

protected:
    void update();

    void clear() noexcept;

private:
    using system_ptr = std::unique_ptr<system_with_priority>;
    using systems_vector = std::vector<system_ptr>;
    using systems_id_vector = std::vector<entt::id_type>;
    using globals_map = entt::dense_map<entt::id_type, std::any>;

    double elapsed_ = 0;
    double delta_ = 0;

    double last_elapsed_ = 0;
    systems_vector systems_;
    systems_vector systems_to_add_;

    systems_id_vector systems_to_remove_;
    entt::registry registry_;

    entt::dispatcher event_dispatcher_;
    globals_map globals_;

    void remove_all_systems() noexcept;

    void discard_pending_events() noexcept;

    void remove_pending_systems() noexcept;

    void sent_events();

    void update_systems();

    void update_time();

    static auto match_type(entt::id_type type_to_remove) noexcept {
        return [type_to_remove](const system_ptr &system) noexcept -> bool { return system->type() == type_to_remove; };
    }

    void add_pending_systems();

    auto remove_system_from_vector(entt::id_type type_to_remove, systems_vector &systems) noexcept -> bool;

    void remove_all_systems_from_vector(systems_vector &systems) noexcept;

    template<typename... Args>
    void recurse_create(entt::entity id, Args... args) {
        helper_create_shift(id, args...);
    }

    template<typename Type, typename... Args>
    [[maybe_unused]] void helper_create_shift(entt::entity id, Type value, Args &&...args) {
        set<Type>(id, value);
        recurse_create(id, args...);
    }

    void helper_create_shift(entt::entity) {}

    static auto sort_by_priority(const system_ptr &lhs, const system_ptr &rhs) noexcept -> bool;
};

} // namespace sneze
