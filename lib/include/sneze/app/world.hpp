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

/**
 * @brief The world class
 * This class is the main class of the engine, it will hold all the entities, globals, components, and systems.
 *
 * It handles the creation of events, and listeners.
 *
 * It will also be the main class that the user will interact with, either in systems, listeners or the own application.
 *
 * @see application
 * @see system
 * @see world::add_entity
 * @see world::add_system
 * @see world::add_listener
 * @see world::set_global
 * @see world::emmit
 */
class world {
    friend class application;

public:
    //! world Default constructor
    world() = default;

    //! world Default destructor
    ~world() = default;

    world(const world &) = delete;
    world(const world &&) = delete;

    auto operator=(const world &) -> world & = delete;
    auto operator=(const world &&) -> world & = delete;

    /**
     * @brief add an entity to the world
     * This function will add an entity to the world, and will return the entity id.
     * @tparam Args the types of the components to add to the entity
     * @param args the values of the components to add to the entity
     * @return the entity id
     * @see world::remove_entity
     */
    template<typename... Args>
    auto add_entity(Args... args) {
        auto entity_id = registry_.create();
        recurse_create(entity_id, args...);
        return entity_id;
    }

    /**
     * @brief remove an entity from the world
     * @param entity the entity id to remove
     * @see world::add_entity
     */
    [[maybe_unused]] void remove_entity(entt::entity entity) {
        registry_.destroy(entity);
    }

    /**
     * @brief get a component from an entity
     * This function will return a reference to the component of the entity.
     *
     * @tparam Type the type of the component to get
     * @param entity the entity id to get the component from
     * @return a reference to the component
     * @see world::set_component
     * @see world::remove_component
     * @see world::has_component
     */
    template<typename Type>
    [[maybe_unused]] auto get_component(entt::entity entity) -> decltype(auto) {
        return registry_.get<Type>(entity);
    }

    /**
     * @brief remove a component from an entity
     * This function will remove a component from an entity.
     *
     * @tparam Type the type of the component to remove
     * @param entity the entity id to remove the component from
     * @see world::set_component
     * @see world::get_component
     * @see world::has_component
     */
    template<typename Type>
    [[maybe_unused]] void remove_component(entt::entity entity) {
        registry_.remove<Type>(entity);
    }

    /**
     * @brief set a component to an entity
     * This function will set a component to an entity.
     *
     * @tparam Type the type of the component to set
     * @tparam Args the types of the arguments to pass to the component
     * @param entity the entity id to set the component to
     * @param args the arguments to pass to the component
     * @see world::get_component
     * @see world::remove_component
     * @see world::has_component
     */
    template<typename Type, typename... Args>
    void set_component(entt::entity entity, Args &&...args) {
        registry_.emplace<Type>(entity, std::forward<Args>(args)...);
    }

    /**
     * @brief check if an entity has a component
     * This function will check if an entity has a component.
     *
     * @tparam Type the type of the component to check
     * @param entity the entity id to check
     * @return true if the entity has the component, false otherwise
     * @see world::set_component
     * @see world::get_component
     * @see world::remove_component
     */
    template<typename Type>
    auto has_component(entt::entity entity) {
        return registry_.try_get<Type>(entity);
    }

    /**
     * @brief get all the entities that have the components
     * This function will return a view of all the entities that have the components.
     *
     * @tparam Types the types of the components to check
     * @return a view of all the entities that have the components
     * @see world::add_entity
     */
    template<typename... Types>
    [[nodiscard]] auto get_entities() {
        return registry_.view<Types...>().each();
    }

    /**
     * @brief tag an entity
     * @tparam TagType the type of the tag to add
     * @param entity the entity id to tag
     * @see world::untag
     * @see world::get_tagged
     */
    template<typename TagType>
    void tag(entt::entity entity) {
        set_component<components::tag<TagType>>(entity);
    }

    /**
     * @brief untag an entity
     * @tparam TagType the type of the tag to remove
     * @param entity the entity id to untag
     * @see world::tag
     * @see world::get_tagged
     */
    template<typename TagType>
    void untag(entt::entity entity) {
        remove_component<components::tag<TagType>>(entity);
    }

    /**
     * @brief return all the entities that have the tag and a set of components
     * @tparam TagType the type of the tag to check
     * @tparam Types the types of the components to check
     * @return a view of all the entities that have the tag
     * @see world::tag
     * @see world::untag
     * @see world::remove_all_tagged
     * @see world::remove_all_tags
     */
    template<typename TagType, typename... Types>
    [[nodiscard]] auto get_tagged() {
        return registry_.view<Types..., components::tag<TagType>>().each();
    }

    /**
     * @brief remove all the entities that have a particular tag
     * @tparam TagType the type of the tag to check
     * @see world::tag
     * @see world::untag
     * @see world::get_tagged
     * @see world::remove_all_tags
     */
    template<typename TagType>
    void remove_all_tagged() {
        for(auto &&[entity]: get_tagged<TagType>()) {
            remove_entity(entity);
        }
    }

    /**
     * @brief remove all the tags of a particular type from all the entities in the world
     * @tparam TagType the type of the tag to remove
     * @see world::tag
     * @see world::untag
     * @see world::get_tagged
     * @see world::remove_all_tagged
     */
    template<typename TagType>
    void remove_all_tags() {
        for(auto &&[entity]: get_tagged<TagType>()) {
            untag<TagType>(entity);
        }
    }

    /**
     * @brief set a global entity
     * A global entity is an entity that is always present in the world and can be accessed by any system, but its only
     * exists in one entity. This function will set the global entity to the given value.
     *
     * @note the global entity must have a default initializer, otherwise the function will fail to compile
     *
     * @tparam Type of the global entity
     * @tparam Args arguments type to pass to the global entity constructor
     * @param args arguments to pass to the global entity constructor
     */
    template<typename Type, typename... Args>
    [[maybe_unused]] void set_global(Args &&...args) {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        auto &&global = get_global<Type>();
        global = Type{std::forward<Args>(args)...};
    }

    /**
     * @brief get a global entity
     * A global entity is an entity that is always present in the world and can be accessed by any system, but its only
     * exists in one entity. This function will return a reference to the global entity.
     *
     * If the global entity does not exist, it will be created with the default initializer.
     *
     * @note the global entity must have a default initializer, otherwise the function will fail to compile
     *
     * @tparam Type of the global entity
     * @return a reference to the global component
     */
    template<typename Type>
    [[maybe_unused]] [[nodiscard]] auto get_global() -> auto & {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        if(auto first = registry_.view<Type, components::tag<global<Type>>>().front(); !(first == entt::null)) {
            return get_component<Type>(first);
        }
        tag<global<Type>>(add_entity(Type{}));
        return get_global<Type>();
    }

    /**
     * @brief remove a global entity
     * A global entity is an entity that is always present in the world and can be accessed by any system, but its only
     * exists in one entity. This function will remove the global entity.
     *
     * @note the global entity must have a default initializer, otherwise the function will fail to compile
     *
     * @tparam Type of the global entity
     */
    template<typename Type>
    [[maybe_unused]] void remove_global() {
        static_assert(std::default_initializable<Type>, "the type must have a default initializer");
        remove_all_tagged<global<Type>>();
    }

    /**
     * @brief sort the entities that have a particular component, using a custom compare function
     *
     * @note the sort function is in-place, so the order of the entities will change
     *
     * @tparam Type the type of the component to sort
     * @tparam Compare the compare function type to use
     * @param compare the compare function to use
     */
    template<typename Type, typename Compare>
    void sort(Compare compare) {
        registry_.sort<Type>(compare);
    }

    /**
     * @brief add a system to the world
     *
     * Create a system and add it to the world. The system will be added with the priority::normal priority.
     *
     * When the world is updated, the system will be updated in the order of the priority.
     *
     * @tparam SystemType the type of the system to add
     * @tparam Args the types of the arguments to pass to the system constructor
     * @param args the arguments to pass to the system constructor
     * @see world::add_system_with_priority
     * @see world::remove_system
     */
    template<typename SystemType, typename... Args>
    [[maybe_unused]] void add_system(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        add_system_with_priority<priority::normal, SystemType>(args...);
    }

    /**
     * @brief add a system to the world with a given priority
     *
     * Create a system and add it to the world. The system will be added with the given priority.
     *
     * note: the priority must be between priority::low (-1000) and priority::high (1000)
     *
     * When the world is updated, the system will be updated in the order of the priority.
     *
     * @tparam Priority the priority of the system
     * @tparam SystemType the type of the system to add
     * @tparam Args the types of the arguments to pass to the system constructor
     * @param args the arguments to pass to the system constructor
     * @see world::add_system
     * @see world::remove_system
     */
    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        static_assert(Priority >= world::priority::low && Priority <= world::priority::high,
                      "priority must be between priority::low (-1000) and priority::high (1000)");
        add_system_with_priority_internal<Priority, SystemType>(args...);
    }

    /**
     * @brief remove a system from the world
     *
     * Remove a system from the world. The system will be removed at the end of the current update.
     *
     * @tparam SystemType the type of the system to remove
     * @see world::add_system
     * @see world::add_system_with_priority
     */
    template<typename SystemType>
    [[maybe_unused]] void remove_system() noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        auto constexpr type_to_remove = entt::type_hash<SystemType>::value();
        systems_to_remove_.push_back(type_to_remove);
    }

    /**
     * @brief add a listener to an event
     * @tparam EventType the type of the event to listen to
     * @tparam Candidate the function to call when the event is dispatched
     * @tparam InstanceType the type of the instance to call the function on
     * @param instance the instance to call the function on
     */
    template<typename EventType, auto Candidate, typename InstanceType>
    void add_listener(InstanceType &&instance) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.sink<EventType>().template connect<Candidate>(instance);
    }

    /**
     * @brief remove all listeners from an event
     * @tparam EventType the type of the event to remove the listeners from
     * @tparam InstanceType the type of the instance to remove the listeners from
     * @param instance the instance to remove the listeners from
     */
    template<typename EventType, typename InstanceType>
    [[maybe_unused]] void remove_listener(InstanceType &&instance) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.sink<EventType>().disconnect(instance);
    }

    /**
     * @brief remove all listeners from an instance
     * @tparam InstanceType the type of the instance to remove the listeners from
     * @param instance the instance to remove the listeners from
     */
    template<typename InstanceType>
    [[maybe_unused]] void remove_listeners(InstanceType &&instance) {
        event_dispatcher_.disconnect(instance);
    }

    /**
     * @brief enqueue an event to be dispatched
     *
     * Enqueue an event to be dispatched. The event will be dispatched at the end of the current update.
     *
     * @note the event must be a descendant of sneze::event
     *
     * @tparam EventType the type of the event to enqueue
     * @tparam Args the types of the arguments to pass to the event constructor
     * @param args the arguments to pass to the event constructor
     * @see world::add_listener
     */
    template<typename EventType, typename... Args>
    void emmit(Args &&...args) {
        static_assert(std::is_base_of<events::event, EventType>::value,
                      "the event must be a descendant of sneze::events::event");
        event_dispatcher_.enqueue<EventType>(this, std::forward<Args>(args)...);
    }

    /**
     * @brief add listener to addition of a component type to an entity
     *
     * note: to remove the listener, use world::remove_listener_to_add_component or
     * world::remove_listener_to_add_component
     *
     * @tparam ComponentType the type of the component to listen to
     * @tparam Candidate the function to call when the event is dispatched
     * @tparam InstanceType the type of the instance to call the function on
     * @param instance the instance to call the function on
     * @see world::remove_listener_to_add_component
     * @see world::remove_component_listeners
     */
    template<typename ComponentType, auto Candidate, typename InstanceType>
    void add_listener_to_add_component(InstanceType &&instance) {
        registry_.on_construct<ComponentType>()
            .template connect<&world::add_listener_to_add_component_internal<ComponentType>>(this);
        add_listener<events::add_component<ComponentType>, Candidate>(instance);
    }

    /**
     * @brief remove listener to addition of a component type to an entity
     *
     * @tparam ComponentType the type of the component to remove the listener from
     * @tparam InstanceType the type of the instance to remove the listener from
     * @param instance the instance to remove the listener from
     * @see world::add_listener_to_add_component
     * @see world::remove_component_listeners
     */
    template<typename ComponentType, typename InstanceType>
    void remove_listener_to_add_component(InstanceType &&instance) {
        registry_.on_construct<ComponentType>()
            .template disconnect<&world::add_listener_to_add_component_internal<ComponentType>>(this);
        remove_listener<events::add_component<ComponentType>>(instance);
    }

    /**
     * @brief remove all listeners to components of type in entity
     *
     * @tparam ComponentType the type of the component to remove the listeners from
     * @tparam InstanceType the type of the instance to remove the listeners from
     * @param instance the instance to remove the listeners from
     * @see world::add_listener_to_add_component
     * @see world::remove_listener_to_add_component
     */
    template<typename ComponentType, typename InstanceType>
    [[maybe_unused]] void remove_component_listeners(InstanceType &&instance) {
        remove_listener_to_add_component<ComponentType>(instance);
    }

protected:
    //! inintialize the world
    void init();

    //! update the world
    void update();

    //! end the world
    void end();

    //! clear the world
    void clear();

    //! priority of the systems
    enum priority : int32_t {
        //! lowest priority
        low = -1000,

        //! normal priority
        normal = 0,

        //! highest priority
        high = 1000,

        //! priority before the applications
        before_applications = high + 1000,

        //! priority after the applications
        after_applications = low - 1000,
    };

    /**
     * @brief add a system to the world with a given priority, for internal use only
     *
     * Create a system and add it to the world. The system will be added with the given priority.
     *
     * note: the priority must be between priority::after_applications and priority::before_applications
     *
     * When the world is updated, the system will be updated in the order of the priority.
     *
     * @tparam Priority the priority of the system
     * @tparam SystemType the type of the system to add
     * @tparam Args the types of the arguments to pass to the system constructor
     * @param args the arguments to pass to the system constructor
     * @see world::add_system
     * @see world::remove_system
     */
    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority_internal(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        static_assert(Priority >= world::priority::after_applications
                          && Priority <= world::priority::before_applications,
                      "priority must be between priority::after_applications and priority::before_applications");
        add_system_with_priority_unrestricted<Priority, SystemType>(args...);
    }

private:
    //! add system with priority, without checking the priority, enqueue the system to be added
    template<int32_t Priority, typename SystemType, typename... Args>
    [[maybe_unused]] void add_system_with_priority_unrestricted(Args... args) noexcept {
        static_assert(std::is_base_of<system, SystemType>::value, "the system to add must implement sneze::system");
        auto constexpr type_hash = entt::type_hash<SystemType>::value();
        systems_to_add_.push_back(
            std::make_unique<system_with_priority>(type_hash, Priority, std::make_unique<SystemType>(args...)));
    }

    //! internal add listener to add component
    template<typename ComponentType>
    void add_listener_to_add_component_internal(entt::registry & /*registry*/, entt::entity entity) {
        auto &component = registry_.get<ComponentType>(entity);
        emmit<events::add_component<ComponentType>>(entity, component);
    }

    //! global tag
    template<typename Type>
    struct global {};

    //! system ptr with priority
    using system_ptr = std::unique_ptr<system_with_priority>;

    //! vector of system ptr
    using systems_vector = std::vector<system_ptr>;

    //! vector of system id
    using systems_id_vector = std::vector<entt::id_type>;

    //! the time that the world was created since the epoch
    game_time start_;

    //! the current time since start
    game_time current_;

    //! the current systems
    systems_vector systems_;

    //! the systems to add
    systems_vector systems_to_add_;

    //! the systems to remove
    systems_id_vector systems_to_remove_;

    //! the registry
    entt::registry registry_;

    //! the event dispatcher
    entt::dispatcher event_dispatcher_;

    //! get the time since the epoch
    [[nodiscard]] static auto since_epoch() -> float;

    //! update the time
    void update_time();

    //! update the systems
    void update_systems();

    //! send the pending events
    void sent_events();

    //! remove all systems
    void remove_all_systems() noexcept;

    //! discard pending events
    void discard_pending_events() noexcept;

    //! remove pending systems
    void remove_pending_systems() noexcept;

    //! match systems by type
    static auto match_type(entt::id_type type_to_remove) noexcept {
        return [type_to_remove](const system_ptr &system) noexcept -> bool { return system->type() == type_to_remove; };
    }

    //! add pending systems
    void add_pending_systems();

    //! helper to remove a system from a vector of systems for a given type
    auto remove_system_from_vector(entt::id_type type_to_remove, systems_vector &systems) noexcept -> bool;

    //! helper to remove all systems from a vector of systems
    void remove_all_systems_from_vector(systems_vector &systems) noexcept;

    //! helper to recursively create components
    template<typename... Args>
    void recurse_create(entt::entity entity_id, Args... args) {
        helper_create_shift(entity_id, args...);
    }

    //! helper to recursively create components, add one component and shift the arguments
    template<typename Type, typename... Args>
    [[maybe_unused]] void helper_create_shift(entt::entity entity_id, Type value, Args &&...args) {
        set_component<Type>(entity_id, value);
        recurse_create(entity_id, args...);
    }

    //! helper to recursively create components, for the last component
    void helper_create_shift(entt::entity) {} // NOLINT(readability-named-parameter)

    //! sorting function by priority
    static auto sort_by_priority(const system_ptr &lhs, const system_ptr &rhs) noexcept -> bool;
};

} // namespace sneze
