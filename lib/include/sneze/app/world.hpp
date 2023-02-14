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

#include <optional>
#include <utility>
#include <vector>

#include "entt/entt.hpp"

namespace sneze {

    class world {
    public:
        typedef entt::entity id;
        world() = default;
        ~world() = default;

        template <typename Type, typename... Args>
        void set( id entity, Args&&... args ) {
            registry_.emplace<Type>( entity, std::forward<Args>( args )... );
        }

        template <typename... Args>
        auto create( Args... args ) {
            auto id = registry_.create();
            recurse_create( id, args... );
            return id;
        }

        template <typename Type>
        auto get( id entity ) {
            registry_.get<Type>( entity );
        }

        template <typename Type>
        auto has( id entity ) {
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

        void sent_events() { event_dispatcher_.update(); }

        template <typename SystemType, typename... Args>
        void add_system( Args... args ) noexcept {
            systems_.push_back( std::make_unique<SystemType>( args... ) );
            systems_.back()->init( *this );
        }

        void update();

    private:
        std::vector<std::unique_ptr<system>> systems_;
        entt::registry registry_;
        entt::dispatcher event_dispatcher_;

        template <typename... Args>
        void recurse_create( world::id id, Args... args ) {
            helper_create_shift( id, args... );
        }

        template <typename Type, typename... Args>
        void helper_create_shift( world::id id, Type value, Args&&... args ) {
            set<Type>( id, value );
            recurse_create( id, args... );
        }

        void helper_create_shift( world::id ) {}
    };

} // namespace sneze