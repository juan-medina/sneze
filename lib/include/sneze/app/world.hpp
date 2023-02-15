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

#include "sneze/systems/system.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "entt/entt.hpp"

namespace sneze {

    class world {
        friend class application;
    public:
        world() = default;
        ~world() = default;

        template <typename Type, typename... Args>
        void set( entt::entity entity, Args&&... args ) {
            registry_.emplace<Type>( entity, std::forward<Args>( args )... );
        }

        template <typename... Args>
        auto create( Args... args ) {
            auto id = registry_.create();
            recurse_create( id, args... );
            return id;
        }

        template <typename Type>
        auto get( entt::entity entity ) {
            registry_.get<Type>( entity );
        }

        template <typename Type>
        auto has( entt::entity entity ) {
            if ( auto ptr = registry_.try_get<Type>( entity ) ) {
                return std::optional<Type>{ *ptr };
            } else {
                return std::optional<Type>( std::nullopt );
            }
        }

        template <typename... Types>
        [[nodiscard]] auto view() {
            return registry_.view<Types...>().each();
        }

        template <typename Type, typename Compare>
        void sort( Compare compare ) {
            registry_.sort<Type>( compare );
        }

        template <typename EventType, auto Candidate, typename InstanceType>
        void add_listener( InstanceType&& instance ) {
            event_dispatcher_.sink<EventType>().template connect<Candidate>( instance );
        }

        template <typename EventType, auto Candidate, typename InstanceType>
        [[maybe_unused]] void remove_listener( InstanceType&& instance ) {
            event_dispatcher_.sink<EventType>().template disconnect<Candidate>( instance );
        }

        template <typename InstanceType>
        [[maybe_unused]] void remove_listeners( InstanceType&& instance ) {
            event_dispatcher_.disconnect( instance );
        }

        template <typename EventType>
        void emmit( EventType event ) {
            event_dispatcher_.enqueue( event );
        }

        enum priority : int32_t {
            highest = std::numeric_limits<int32_t>::max(),
            high = highest >> 1,

            lowest = std::numeric_limits<int32_t>::min(),
            low = lowest >> 1,

            normal = 0,
        };

        template <typename SystemType, typename... Args>
        [[maybe_unused]] void add_system( Args... args ) noexcept {
            add_system_with_priority<SystemType>( priority::normal, args... );
        }

        template <typename SystemType, typename... Args>
        [[maybe_unused]] void add_system_with_priority( int32_t priority, Args... args ) noexcept {
            systems_to_add_.push_back( std::make_unique<system_with_priority>(
                entt::type_hash<SystemType>::value(), priority, std::make_unique<SystemType>( args... ) ) );
        }

        template <typename SystemType>
        [[maybe_unused]] void remove_system() noexcept {
            const entt::id_type type_to_remove = entt::type_hash<SystemType>::value();
            systems_to_remove_.push_back( type_to_remove );
        }

    protected:

        void update();

        void clear() noexcept;

    private:

        void remove_all_systems() noexcept;

        void discard_pending_events() noexcept;

        void remove_pending_systems() noexcept;

        void sent_events();

        void update_systems();

        class system_with_priority {
        public:
            system_with_priority( entt::id_type type, std::int32_t priority, std::unique_ptr<system> system ):
                type_{ type }, system_( std::move( system ) ), priority_( priority ) {}

            void init( world& world ) { system_->init( world ); }
            void update( world& world ) { system_->update( world ); }
            void end( world& world ) { system_->end( world ); }

            [[nodiscard]] inline auto& priority() const { return priority_; }
            [[nodiscard]] inline auto& type() const { return type_; }

        private:
            entt::id_type type_;
            std::int32_t priority_;
            std::unique_ptr<system> system_;
        };

        typedef std::unique_ptr<system_with_priority> system_ptr;
        typedef std::vector<system_ptr> systems_vector;
        typedef std::vector<entt::id_type> systems_id_vector;

        systems_vector systems_;
        systems_vector systems_to_add_;
        systems_id_vector systems_to_remove_;

        entt::registry registry_;
        entt::dispatcher event_dispatcher_;

        void add_pending_systems() noexcept;

        bool remove_system_from_vector( entt::id_type type_to_remove, systems_vector& systems ) noexcept;

        void remove_all_systems_from_vector( systems_vector& systems ) noexcept;

        template <typename... Args>
        void recurse_create( entt::entity id, Args... args ) {
            helper_create_shift( id, args... );
        }

        template <typename Type, typename... Args>
        void helper_create_shift( entt::entity id, Type value, Args&&... args ) {
            set<Type>( id, value );
            recurse_create( id, args... );
        }

        void helper_create_shift( entt::entity ) {}

        static bool sort_by_priority( const system_ptr& lhs, const system_ptr& rhs ) noexcept;
    };

} // namespace sneze