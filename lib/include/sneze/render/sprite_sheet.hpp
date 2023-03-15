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
#include <string>
#include <unordered_map>

#include "../components/geometry.hpp"

#include "resource.hpp"

namespace sneze {

class render;

struct frame {
    components::rect rect;      // cppcheck-suppress unusedStructMember
    components::position pivot; // cppcheck-suppress unusedStructMember
};

class sprite_sheet: public resource {
public:
    explicit sprite_sheet(class render *render): resource(render){};

    ~sprite_sheet() override;

    sprite_sheet(const sprite_sheet &) = delete;
    sprite_sheet(sprite_sheet &&) = delete;

    auto operator=(const sprite_sheet &) -> sprite_sheet & = delete;
    auto operator=(sprite_sheet &&) -> sprite_sheet & = delete;

    [[nodiscard]] auto init(const std::string &uri) -> result<> override;

    auto end() -> void override;

private:
    std::unordered_map<std::string, frame> frames_ = {};
    std::string texture_;
    std::string sprite_sheet_directory_;
};

} // namespace sneze
