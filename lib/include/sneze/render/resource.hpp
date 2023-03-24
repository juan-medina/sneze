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
#include "../platform/type_name.hpp"

namespace sneze {

class render;

/**
 * @brief Base class for all resources
 * @tparam Args... Arguments to be passed to the init method of the resource
 * @see sneze::render
 * @see sneze::resources_cache
 */
template<typename... Args>
class resource {
public:
    /**
     * @brief Construct a new resource object
     * @param render The render object that will be used to create the resource
     */
    explicit resource(render *render): render_{render} {};

    virtual ~resource() = default;

    /**
     * @brief Initialize the resource
     * @param uri The URI of the resource
     * @param args Arguments to be passed to the resource init method
     * @return true if the resource was initialized successfully, error otherwise
     */
    [[nodiscard]] virtual auto init(const std::string &uri, Args... args) -> result<> = 0;

    //! Destroy the resource
    virtual void end() = 0;

    resource(const resource &) = delete;
    resource(resource &&) = delete;
    auto operator=(const resource &) -> resource & = delete;
    auto operator=(resource &&) -> resource & = delete;

protected:
    /**
     * @brief Get the render object
     * @return the render object
     */
    [[maybe_unused]] [[nodiscard]] auto get_render() const noexcept -> render * {
        return render_;
    };

private:
    //! The render object that will be used to create the resource
    class render *render_{nullptr};
};

/**
 * @brief Helper class to store the resource and the count of how many times it has been loaded
 * @tparam Type the type of the resource
 * @tparam Args the types of the arguments to be passed to the init method of the resource
 */
template<typename Type, typename... Args>
struct resource_entry {
    static_assert(std::is_base_of<resource<Args...>, Type>::value,
                  "resource entry must be for a descent of sneze::resource");
    //! The resource data
    std::shared_ptr<Type> data{nullptr}; // cppcheck-suppress unusedStructMember
    //! The count of how many times it has been loaded
    int count{0}; // cppcheck-suppress unusedStructMember
};

/**
 * @brief Helper class to cache resources
 * @details This class is used to cache resources, so that they are not loaded more than once, when unloading a resource
 * the count is decreased, and when it reaches 0 the resource is destroyed
 * @note resources must inherit from sneze::resource
 * @tparam Type the type of the resource
 * @tparam Args the types of the arguments to be passed to the init method of the resource
 * @see sneze::resource
 */
template<typename Type, typename... Args>
class resources_cache {
    static_assert(std::is_base_of<resource<Args...>, Type>::value,
                  "resources cache must be for a descent of sneze::resource");

public:
    /**
     * @brief Construct a new resources cache object
     * @param render The render object that will be used to create the resource
     */
    explicit resources_cache(render *render): render_{render} {}
    ~resources_cache() = default;

    resources_cache(const resources_cache &) = delete;
    resources_cache(resources_cache &&) = delete;
    auto operator=(const resources_cache &) -> resources_cache & = delete;
    auto operator=(resources_cache &&) -> resources_cache & = delete;

    /**
     * @brief Load a resource
     * If the resource is already loaded, the count is increased, otherwise the resource is loaded
     * @param uri The URI of the resource
     * @param args Arguments to be passed to the resource
     * @return true if the resource was loaded successfully, error otherwise
     */
    [[nodiscard]] auto load(const std::string &uri, Args... args) -> result<> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            it_resource->second.count++;
            logger::trace("request to load resource<{}>: {}, increase count to: {}",
                          type_name<Type>(),
                          uri,
                          it_resource->second.count);
        } else {
            auto new_resource = std::make_shared<Type>(render_);
            if(auto err = new_resource->init(uri, args...).ko(); err) {
                new_resource.reset();
                logger::error("fail to load resource<{}>: {}", type_name<Type>(), uri);
                return error("Fail to load resource.", *err);
            }
            logger::trace("new resource<{}> loaded: {}, set count to: 1", type_name<Type>(), uri);
            resources_.insert({uri, resource_entry<Type, Args...>{new_resource, 1}});
        }
        return true;
    }

    /**
     * @brief Unload a resource
     * @details If the resource is loaded more than once, the count is decreased, otherwise the resource is unloaded
     * @param uri The URI of the resource
     * @return true if the resource was unloaded successfully, error otherwise
     */
    [[nodiscard]] auto unload(const std::string &uri) -> result<> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            it_resource->second.count--;
            logger::trace("request to unload resource<{}>: {}, decrease count to: {}",
                          type_name<Type>(),
                          uri,
                          it_resource->second.count);
            if(it_resource->second.count == 0) {
                logger::trace("resource<{}> unloaded: {} since count is 0", type_name<Type>(), uri);
                resources_.erase(it_resource);
            }
            return true;
        }
        logger::error("fail to unload resource<{}>: {}", type_name<Type>(), uri);
        return error("Fail to unload resource.");
    }

    /**
     * @brief Get a resource
     * @note if it is not loaded, an error is returned
     * @param uri The URI of the resource
     * @return true if the resource is loaded, error otherwise
     */
    [[nodiscard]] auto get(const std::string &uri) -> result<std::shared_ptr<Type>, error> {
        if(auto it_resource = resources_.find(uri); it_resource != resources_.end()) {
            return it_resource->second.data;
        }
        logger::error("fail to get a resource<{}> not loaded: {}", type_name<Type>(), uri);
        return error("Fail to get resource.");
    }

    /**
     * @brief Clear the cache
     */
    void clear() {
        resources_.clear();
    }

private:
    //! resources entries
    std::unordered_map<std::string, resource_entry<Type, Args...>> resources_ = {};
    //! The render object that will be used to create the resource
    render *render_{nullptr};
};

} // namespace sneze