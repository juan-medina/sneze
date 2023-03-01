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
#include <unordered_map>

#include "../platform/logger.hpp"
#include "../platform/result.hpp"
#include "../platform/traits.hpp"

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
    [[maybe_unused]] [[nodiscard]] auto render() const noexcept -> render *const {
        return render_;
    };

private:
    [[maybe_unused]] class render *render_{nullptr};
};

template<implements_interface<resource> Type>
struct resource_entry {
    std::shared_ptr<Type> resource{nullptr}; // cppcheck-suppress unusedStructMember
    int count{0};                            // cppcheck-suppress unusedStructMember
};

template<implements_interface<resource> Type>
class resources_cache {
public:
    [[maybe_unused]] explicit resources_cache(render *render): render_{render} {}
    ~resources_cache() = default;

    resources_cache(const resources_cache &) = delete;
    resources_cache(resources_cache &&) = delete;
    auto operator=(const resources_cache &) -> resources_cache & = delete;
    auto operator=(resources_cache &&) -> resources_cache & = delete;

    auto load(const std::string &uri) -> result<> {
        if(auto it = resources_.find(uri); it != resources_.end()) {
            it->second.count++;
            logger::debug("request to load resource: {}, increase count to: {}", uri, it->second.count);
        } else {
            auto new_resource = std::make_shared<Type>(render_);
            if(auto err = new_resource->init(uri).ko(); err) {
                new_resource.reset();
                logger::error("fail to load resource: {}", uri);
                return error("Fail to load resource.", *err);
            }
            logger::debug("new resource loaded: {}, set count to: 1", uri);
            resources_.insert({uri, resource_entry<Type>{new_resource, 1}});
        }
        return true;
    }

    auto unload(const std::string &uri) -> result<> {
        if(auto it = resources_.find(uri); it != resources_.end()) {
            it->second.count--;
            logger::debug("request to unload resource: {}, decrease count to: {}", uri, it->second.count);
            if(it->second.count == 0) {
                logger::debug("resource unloaded: {} since count is 0", uri);
                resources_.erase(it);
            }
            return true;
        }
        logger::error("fail to unload resource: {}", uri);
        return error("Fail to unload resource.");
    }

    auto get(const std::string &uri) -> result<std::shared_ptr<Type>, error> {
        if(auto it = resources_.find(uri); it != resources_.end()) {
            return it->second.resource;
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