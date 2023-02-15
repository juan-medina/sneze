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

namespace sneze {

    void world::update() {
        update_systems();
        sent_events();
    }

    void world::update_systems() {
        if ( !systems_to_add_.empty() ) { add_pending_systems(); }

        if ( !systems_to_remove_.empty() ) { remove_pending_systems(); }

        for ( auto& system : systems_ ) {
            system->update( *this );
        }
    }

    void world::add_pending_systems() noexcept {
        logger::debug( "adding systems" );
        for ( auto& system : systems_to_add_ ) {
            systems_.push_back( std::move( system ) );
            systems_.back()->init( *this );
        }
        systems_to_add_.clear();
        logger::debug( "sorting systems" );
        std::sort( systems_.begin(), systems_.end(), world::sort_by_priority );
    }

    void world::remove_pending_systems() noexcept {
        logger::debug( "checking if we need to delete any system" );
        for ( auto type_to_remove : systems_to_remove_ ) {
            if ( !remove_system_from_vector( type_to_remove, systems_ ) ) {
                remove_system_from_vector( type_to_remove, systems_to_add_ );
            }
        }
        systems_to_remove_.clear();
    }

    bool world::remove_system_from_vector( entt::id_type type_to_remove, systems_vector& systems ) noexcept {
        auto it_systems = std::find_if( systems.begin(), systems.end(), [type_to_remove]( const auto& system ) {
            return system->type() == type_to_remove;
        } );

        if ( it_systems != systems.end() ) {
            it_systems->get()->end( *this );
            systems.erase( it_systems );
            return true;
        }

        return false;
    }

    void world::remove_all_systems() noexcept {
        remove_all_systems_from_vector( systems_ );
        remove_all_systems_from_vector( systems_to_add_ );
    }

    void world::remove_all_systems_from_vector( systems_vector& systems ) noexcept {
        for ( auto& system : systems )
            system->end( *this );
        systems.clear();
    }

    bool world::sort_by_priority( const system_ptr& lhs, const system_ptr& rhs ) noexcept {
        return lhs->priority() > rhs->priority();
    }

    void world::sent_events() { event_dispatcher_.update(); }

    void world::discard_pending_events() noexcept { event_dispatcher_.clear(); }

    void world::clear() noexcept {
        logger::info( "world clean up" );

        logger::debug( "discarding pending events" );
        discard_pending_events();

        logger::debug( "removing systems pending to be deleted" );
        remove_pending_systems();

        logger::debug( "removing all systems" );
        remove_all_systems();

        logger::debug( "resetting dispatcher" );
        event_dispatcher_ = entt::dispatcher{};

        logger::debug( "clear registry" );
        registry_.clear();
    }
} // namespace sneze
