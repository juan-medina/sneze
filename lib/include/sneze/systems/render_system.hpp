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

#include <memory>
#include <utility>

#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../events/events.hpp"
#include "../systems/system.hpp"

namespace sneze {

class render;

class render_system final: public sneze::system {
public:
    explicit render_system(std::shared_ptr<sneze::render> render);

    void update(sneze::world *world) override;

    void init(sneze::world *world) override;

    void end(sneze::world *world) override;

private:
    std::shared_ptr<sneze::render> render_;

    static inline auto sort_by_depth(const components::renderable &lhs, const components::renderable &rhs) {
        return lhs.depth > rhs.depth;
    }

    void toggle_fullscreen(events::toggle_fullscreen const &event) noexcept;
};

} // namespace sneze
