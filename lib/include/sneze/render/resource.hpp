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
#include <type_traits>
#include <unordered_map>

#include "../platform/logger.hpp"
#include "../platform/result.hpp"

namespace sneze {

class render;

class resource {
public:
    explicit resource(render *render): render_{render} {};

    virtual ~resource() = default;

    virtual auto init(const std::string &uri) -> result<> = 0;

    virtual auto end() -> void = 0;

    resource(const resource &) = delete;
    resource(resource &&) = delete;
    auto operator=(const resource &) -> resource & = delete;
    auto operator=(resource &&) -> resource & = delete;

protected:
    [[maybe_unused]] [[nodiscard]] auto get_render() const noexcept -> render * {
        return render_;
    };

private:
    class render *render_{nullptr};
};

template<typename Type>
struct resource_entry {
    static_assert(std::is_base_of<resource, Type>::value, "resource entry must be for a descent of sneze::resource");
    std::shared_ptr<Type> data{nullptr}; // cppcheck-suppress unusedStructMember
    int count{0};                        // cppcheck-suppress unusedStructMember
};

template<typename Type>
class resources_cache {
    static_assert(std::is_base_of<resource, Type>::value, "resources cache must be for a descent of sneze::resource");

public:
    [[maybe_unused]] explicit resources_cache(render *render): render_{render} {}
    ~resources_cache() = default;

    resources_cache(const resources_cache &) = delete;
    resources_cache(resources_cache &&) = delete;
    auto operator=(const resources_cache &) -> resources_cache & = delete;
    auto operator=(resources_cache &&) -> resources_cache & = delete;

    auto load(const std::string &uri) -> result<> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            it_resource->second.count++;
            logger::trace("request to load resource: {}, increase count to: {}", uri, it_resource->second.count);
        } else {
            auto new_resource = std::make_shared<Type>(render_);
            if(auto err = new_resource->init(uri).ko(); err) {
                new_resource.reset();
                logger::error("fail to load resource: {}", uri);
                return error("Fail to load resource.", *err);
            }
            logger::trace("new resource loaded: {}, set count to: 1", uri);
            resources_.insert({uri, resource_entry<Type>{new_resource, 1}});
        }
        return true;
    }

    auto unload(const std::string &uri) -> result<> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            it_resource->second.count--;
            logger::trace("request to unload resource: {}, decrease count to: {}", uri, it_resource->second.count);
            if(it_resource->second.count == 0) {
                logger::trace("resource unloaded: {} since count is 0", uri);
                resources_.erase(it_resource);
            }
            return true;
        }
        logger::error("fail to unload resource: {}", uri);
        return error("Fail to unload resource.");
    }

    auto get(const std::string &uri) -> result<std::shared_ptr<Type>, error> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            return it_resource->second.data;
        }
        logger::error("fail to get a resource not loaded: {}", uri);
        return error("Fail to get resource.");
    }

    void clear() {
        resources_.clear();
    }

private:
    std::unordered_map<std::string, resource_entry<Type>> resources_ = {};
    render *render_{nullptr};
};

} // namespace sneze