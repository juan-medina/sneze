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

#include <entt/fwd.hpp>

namespace sneze {

class world;

/**
 * @brief base class for all systems
 * @see sneze::world
 * @see sneze::world::add_system
 * @see sneze::world::add_system_with_priority
 */
struct system {
    //! initialize the system
    virtual void init(world *world) = 0;
    //! update the system
    virtual void update(world *world) = 0;
    //! shutdown the system
    virtual void end(world *world) = 0;

    system() = default;
    virtual ~system() = default;

    system(const system &) = delete;
    system(const system &&) = delete;

    auto operator=(const system &) -> system & = delete;
    auto operator=(const system &&) -> system & = delete;
};

/**
 * @brief system with priority
 * @see sneze::world::add_system_with_priority
 */
class system_with_priority {
public:
    /**
     * @brief Construct a new system with priority object
     * @param type the type id of the system
     * @param priority the priority of the system
     * @param system the system
     */
    system_with_priority(const entt::id_type type, std::int32_t priority, std::unique_ptr<system> system)
        : type_{type}, system_{std::move(system)}, priority_{priority} {}

    //! initialize the system
    void init(world *world) {
        system_->init(world);
    }

    //! update the system
    void update(world *world) {
        system_->update(world);
    }

    //! shutdown the system
    void end(world *world) {
        system_->end(world);
    }

    /**
     * @brief Get the priority of the system
     * @return the priority of the system
     */
    [[nodiscard]] inline auto priority() const -> auto & {
        return priority_;
    }

    /**
     * @brief Get the type of the system
     * @return the type of the system
     */
    [[nodiscard]] inline auto type() const -> auto & {
        return type_;
    }

private:
    //! the type id of the system
    entt::id_type type_{};
    //! the system
    std::unique_ptr<system> system_{};
    //! the priority of the system
    std::int32_t priority_{};
};

} // namespace sneze
