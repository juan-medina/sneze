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
        if ( !systems_to_add_.empty() ) {
            logger::debug( "adding systems" );
            for ( auto& system : systems_to_add_ ) {
                systems_.push_back( std::move( system ) );
                systems_.back()->init( *this );
            }
            systems_to_add_.clear();
            logger::debug( "sorting systems" );
            std::sort( systems_.begin(), systems_.end(), world::sort_by_priority );
        }

        for ( auto& system : systems_ ) {
            system->update( *this );
        }
    }
    void world::sent_events() { event_dispatcher_.update(); }
} // namespace sneze
