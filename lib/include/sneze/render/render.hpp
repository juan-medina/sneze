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

#include <cstddef>
#include <filesystem>
#include <istream>
#include <memory>
#include <span>

#include "../app/world.hpp"
#include "../components/geometry.hpp"
#include "../components/renderable.hpp"
#include "../components/ui.hpp"
#include "../platform/result.hpp"

#include "font.hpp"
#include "sprite_sheet.hpp"
#include "texture.hpp"

struct SDL_Renderer;
struct SDL_Window;
struct SDL_RWops;

namespace sneze {

/**
 * @brief render class
 *
 * this class have all the render related functions, including creating the window, initializing SDL and
 * handling resources.
 *
 * This class is constructed by the application class, and is not meant to be used directly.
 * @see application
 */
class render {
public:
    //! Construct a new render object
    render(): fonts_{this}, textures_{this}, sprite_sheets_{this} {};
    ~render() = default;

    render(const render &) = delete;
    render(render &&) = delete;

    auto operator=(const render &) -> render & = delete;
    auto operator=(render &&) -> render & = delete;

    /**
     * @brief initialize the render
     * @param size window size in pixels
     * @param logical logical size in points
     * @param fullscreen flag to set the window to fullscreen
     * @param monitor number of the monitor to use
     * @param title title of the window
     * @param icon icon of the window, file path
     * @param color clear color
     * @return true if the render was initialized correctly or error if not
     */
    [[nodiscard]] auto init(const components::size &size,
                            const components::size &logical,
                            const bool &fullscreen,
                            const int &monitor,
                            const std::string &title,
                            const std::string &icon,
                            const components::color &color) -> result<>;

    //! end the render
    void end();

    //! begin a new frame
    void begin_frame();

    //! end the current frame
    void end_frame();

    /**
     * @brief load a font
     *
     * load a font from a file path, the font will be cached, so if the font is already loaded, it will
     * return the cached font.
     *
     * @param font_path path of the font to load
     * @return true if the font was loaded correctly or error if not
     */
    [[maybe_unused]] [[nodiscard]] auto load_font(const std::string &font_path) -> result<>;

    /**
     * @brief unload a font, if the font is not loaded, it will return an error
     *
     * this function will remove the font from the cache, if there are no more references to the font,
     * it will be deleted.
     *
     * @param font_path the path of the font to unload
     * @return true if the font was unloaded correctly or error if not
     */
    [[maybe_unused]] auto unload_font(const std::string &font_path) -> result<>;

    /**
     * @brief load a texture
     *
     * load a texture from a file path, the texture will be cached, so if the texture is already loaded,
     * it will return the cached texture.
     *
     * @param texture_path path of the texture to load
     * @return true if the texture was loaded correctly or error if not
     */
    [[maybe_unused]] [[nodiscard]] auto load_texture(const std::string &texture_path) -> result<>;

    /**
     * @brief unload a texture, if the texture is not loaded, it will return an error
     *
     * this function will remove the texture from the cache, if there are no more references to the
     * texture, it will be deleted.
     *
     * @param texture_path path of the texture to unload
     * @return true if the texture was unloaded correctly or error if not
     */
    [[maybe_unused]] auto unload_texture(const std::string &texture_path) -> result<>;

    /**
     * @brief load a sprite sheet
     *
     * load a sprite sheet from a file path, the sprite sheet will be cached, so if the sprite sheet is
     * already loaded, it will return the cached sprite sheet.
     *
     * @param sprite_sheet_path path of the sprite sheet to load
     * @return true if the sprite sheet was loaded correctly or error if not
     */
    [[maybe_unused]] [[nodiscard]] auto load_sprite_sheet(const std::string &sprite_sheet_path) -> result<>;

    /**
     * @brief unload a sprite sheet, if the sprite sheet is not loaded, it will return an error
     *
     * this function will remove the sprite sheet from the cache, if there are no more references to
     * the sprite sheet, it will be deleted.
     *
     * @param sprite_sheet_path path of the sprite sheet to unload
     * @return true if the sprite sheet was unloaded correctly or error if not
     */
    [[maybe_unused]] auto unload_sprite_sheet(const std::string &sprite_sheet_path) -> result<>;

    /**
     * @brief load a sprite
     *
     * load a sprite from a file path, the sprite will be cached, so if the sprite is already loaded,
     * it will return the cached sprite.
     *
     * @param sprite_path path of the sprite to load
     * @return true if the sprite was loaded correctly or error if not
     */
    [[maybe_unused]] [[nodiscard]] auto load_sprite(const std::string &sprite_path) -> result<>;

    /**
     * @brief unload a sprite, if the sprite is not loaded, it will return an error
     *
     * this function will remove the sprite from the cache, if there are no more references to the
     * sprite, it will be deleted.
     *
     * @param sprite_path path of the sprite to unload
     * @return true if the sprite was unloaded correctly or error if not
     */
    [[maybe_unused]] auto unload_sprite(const std::string &sprite_path) -> result<>;

    /**
     * @brief draw a label
     * @param label the label to draw
     * @param from the position to draw the label
     * @param color the color of the label
     * @see components::label
     */
    void draw_label(const components::label &label, const components::position &from, const components::color &color);

    /**
     * @brief draw a line
     * @param line the line to draw
     * @param from the position to draw the line
     * @param color the color of the line
     * @see components::line
     */
    void draw_line(const components::line &line, const components::position &from, const components::color &color);

    /**
     * @brief draw a box
     * @param box the box to draw
     * @param from the position to draw the box
     * @param color the color of the box
     */
    void draw_box(const components::box &box, const components::position &from, const components::color &color);

    /**
     * @brief draw a solid box
     * @param box the box to draw
     * @param from the position to draw the box
     * @param color the color of the box
     */
    void
    draw_solid_box(const components::solid_box &box, const components::position &from, const components::color &color);

    /**
     * @brief draw a border box
     * @param box the box to draw
     * @param from the position to draw the box
     * @param color the color of the box
     */
    void draw_border_box(const components::border_box &box,
                         const components::position &from,
                         const components::color &color);

    /**
     * @brief draw a sprite
     * @param sprite the sprite to draw
     * @param from the position to draw the sprite
     * @param color the color of the sprite
     */
    void draw_sprite(components::sprite &sprite, const components::position &from, const components::color &color);

    /**
     * @brief get the window size
     * @return the window size
     */
    [[nodiscard]] auto get_window_size() -> components::size;

    /**
     * @brief get the logical size
     * @return the logical size
     */
    [[nodiscard]] auto get_logical_size() -> components::rect;

    /**
     * @brief convert a window position into a logical position
     * @param position
     * @return
     */
    [[nodiscard]] auto window_to_logical(const components::position &position) -> components::position;

    /**
     * @brief convert a window size into a logical size
     * @param size
     * @return
     */
    [[nodiscard]] auto window_to_logical(const components::size &size) -> components::rect;

    /**
     * @brief get the monitor where the window is
     * @return the monitor where the window is
     */
    [[nodiscard]] auto get_monitor() const -> int;

    /**
     * @brief get if the window is fullscreen
     * @return true if the window is fullscreen, false otherwise
     */
    [[nodiscard]] auto is_fullscreen() const {
        return fullscreen_;
    }

    /**
     * @brief get the sdl renderer
     * @return the sdl renderer
     */
    [[nodiscard]] auto get_sdl_renderer() -> SDL_Renderer * {
        return renderer_;
    }

    //! @brief toggle between fullscreen and windowed mode
    void toggle_fullscreen();

    /**
     * get a sdl rw operations from a file path
     * @param path the path of the file
     * @note this support embedded files
     * @return the sdl rw operations
     */
    [[nodiscard]] auto get_sdl_rwops(const std::string &path) -> SDL_RWops *;

    /**
     * @brief
     * @param rwops the sdl rw operations
     */
    static void free_sdl_rwops(SDL_RWops *rwops);

    /**
     * @brief get a istream from a file path
     * @param path the path of the file
     * @note this support embedded files
     * @return the istream
     */
    [[maybe_unused]] [[nodiscard]] auto get_istream(const std::string &path) -> std::unique_ptr<std::istream>;

    /**
     * @brief check if a file exists
     * @param path the path of the file
     * @return true if the file exists, false otherwise
     */
    [[maybe_unused]] [[nodiscard]] auto file_exists(const std::string &path) -> bool;

    /**
     * @brief get the parent of a given path
     * @param path the path
     * @return the parent path of the given path
     */
    [[maybe_unused]] [[nodiscard]] auto get_parent(const std::string &path) -> std::filesystem::path;

    friend class font;
    friend class sprite_sheet;

protected:
    /**
     * @brief get a texture
     * @param texture_path the path of the texture
     * @return a shared pointer to the texture
     */
    [[nodiscard]] auto get_texture(const std::string &texture_path) -> std::shared_ptr<texture>;

private:
    //! the font cache
    resources_cache<font> fonts_;
    //! the texture cache
    resources_cache<texture> textures_;
    //! the sprite sheet cache
    resources_cache<sprite_sheet, bool> sprite_sheets_;
    //! the clear color
    components::color clear_color_ = components::color::black;
    //! if the window is fullscreen
    bool fullscreen_ = false;
    //! the SDL window
    SDL_Window *window_ = {nullptr};
    //! the SDL renderer
    SDL_Renderer *renderer_ = {nullptr};
    //! embedded data map
    std::unordered_map<std::string, std::span<std::byte const>> embedded_data_;

    /**
     * @brief get a font
     * @param font_path the path of the font
     * @note this support embedded files
     * @return a shared pointer to the font
     */
    [[nodiscard]] auto get_font(const std::string &font_path) -> std::shared_ptr<font>;

    /**
     * @brief get a sprite sheet
     * @param sprite_sheet_path the path of the sprite sheet
     * @param load_textures if the textures should be loaded
     * @note this support embedded files
     * @return a shared pointer to the sprite sheet
     */
    [[nodiscard]] auto get_sprite_sheet(const std::string &sprite_sheet_path) -> std::shared_ptr<sprite_sheet>;

    /**
     * @brief get the preferred SDL driver
     * @note this is used to get the best driver for the current platform, the current priority is:
     * 1. direct3d12
     * 2. direct3d11
     * 3. direct3d
     * 4. metal
     * 5. opengl
     * 6. opengles2
     * 7. opengles
     * @return the preferred SDL driver
     */
    [[nodiscard]] static auto preferred_driver() -> int;

    /**
     * @brief fill a vector of points with triangles
     * @param points a vector of points
     * @param color the fill color
     */
    void fill_points_with_triangles(const std::vector<components::position> &points, const components::color &color);

    /**
     * @brief set the window icon
     * @param icon the path of the icon
     * @return true if the icon was set, error otherwise
     */
    [[nodiscard]] auto set_icon(const std::string &icon) -> result<>;

    /**
     * @brief get a sdl rw operations from a span of bytes
     * @param data the span of bytes
     * @return the sdl rw operations
     */
    [[nodiscard]] static auto get_embedded_sdl_rwops(std::span<std::byte const> &data) -> SDL_RWops *;

    /**
     * @brief get a istream from a span of bytes
     * @param data the span of bytes
     * @return the istream
     */
    [[nodiscard]] static auto get_embedded_istream(std::span<std::byte const> &data) -> std::unique_ptr<std::istream>;

    //! @brief initialize the embedded data
    void init_embedded_data();

    /**
     * @brief add data to the embedded data map
     * @param path the path of the data
     * @param data the data
     */
    void add_to_embedded_data(const std::string &path, const std::span<std::byte const> &data);

    /**
     * @brief get data from the embedded data map
     * @param path the path of the data
     * @return the data
     */
    [[nodiscard]] auto get_from_embedded_data(const std::string &path) -> std::optional<std::span<std::byte const>>;
};

} // namespace sneze
