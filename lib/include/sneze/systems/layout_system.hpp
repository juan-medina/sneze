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

#include "../components/ui.hpp"
#include "../events/events.hpp"

#include "system.hpp"

namespace sneze {

/**
 * @brief layout system
 *
 * this system will handle the layout of elements in the screen
 *
 * @note this system will only work with entities that have the anchor component
 * @see components::anchor
 */
class layout_system: public system {
public:
    /**
     * @brief initialize the system
     * @param world the world that owns this system
     */
    void init(world *world) override;

    /**
     * @brief shutdown the system
     * @param world the world that owns this system
     */
    void end(world *world) override;

    /**
     * @brief update the system
     * @param world the world that owns this system
     */
    void update(world *world) override;

private:
    //! logical size of the screen
    components::rect logical_ = {{0, 0}, {0, 0}};

    //! window resized event handler
    void window_resized(const events::window_resized &event);

    //! add component anchor event handler
    void add_component_anchor(events::add_component<components::anchor> const &event);

    //! calculate the layout of the entity
    void calculate_layout(world *world, entt::entity entity, const components::anchor &anchor) const;
};

} // namespace sneze