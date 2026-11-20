//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Window.h
///
/// @brief Defines the Window class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_WINDOW_H
#define FFG_ENGINE_WINDOW_H

#include "Constants.h"
#include "WindowConfig.h"
#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace FFG::ENGINE {

/// @class Window
///
/// @brief An interface for the #Engine's window and rendering system.
class Window {
private:
    SDL_Window *window = nullptr;
    SDL_GLContext glContext = nullptr;
    int window_w = 0;
    int window_h = 0;
    const char *initial_window_title = nullptr;
    int initial_window_size_w = DEFAULT_FFG_WINDOW_SIZE_W;
    int initial_window_size_h = DEFAULT_FFG_WINDOW_SIZE_H;
    bool initial_window_resizeable = DEFAULT_FFG_WINDOW_RESIZEABLE;
    bool initial_window_fullscreen = DEFAULT_FFG_WINDOW_FULLSCREEN;
    bool initial_window_vsync = DEFAULT_FFG_WINDOW_VSYNC;
    bool initial_window_adaptive_vsync = DEFAULT_FFG_WINDOW_ADAPTIVE_VSYNC;
    GLfloat initial_renderer_clear_color_r = DEFAULT_FFG_RENDERER_CLEAR_R;
    GLfloat initial_renderer_clear_color_g = DEFAULT_FFG_RENDERER_CLEAR_G;
    GLfloat initial_renderer_clear_color_b = DEFAULT_FFG_RENDERER_CLEAR_B;
    GLfloat initial_renderer_clear_color_a = DEFAULT_FFG_RENDERER_CLEAR_A;

protected:
    /// @cond
    void init();
    void exit();
    void update_window_size(SDL_Event *event);
    static void clear();
    void swap();
    /// @endcond

public:
    /// @brief Sets the initial window title.
    ///
    /// @note This should only be called before engine initialization It has has no effect otherwise.
    ///
    /// @param title The title of the window.
    void set_initial_window_title(const char *title) { this->initial_window_title = title; }

    /// @brief Sets the initial window size in pixels.
    ///
    /// @details This sets the initial size of the window itself, not the window's draw area. You will need to query the
    /// actual size of the window's draw area using #get_window_size(). If your application starts with its window in
    /// fullscreen mode (i.e. you call #set_initial_window_fullscreen() and pass it `true`) it is still important to
    /// call this function to set the fallback window size for when/if your application exits fullscreen mode.
    ///
    /// @note This should only be called before engine initialization It has has no effect otherwise.
    ///
    /// @param width The width, in pixels.
    /// @param height The height, in pixels.
    void set_initial_window_size(int width, int height) {
        this->initial_window_size_w = width;
        this->initial_window_size_h = height;
    }

    /// @brief Sets if the window is initially resizeable or not.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param resizeable `true` if the window should be resizeable, `false`
    /// otherwise.
    void set_initial_window_resizeable(bool resizeable) { this->initial_window_resizeable = resizeable; }

    /// @brief Sets if the window is initially fullscreen or not.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param fullscreen `true` for fullscreen, `false` for not.
    void set_initial_window_fullscreen(bool fullscreen) { this->initial_window_fullscreen = fullscreen; }

    /// @brief Sets the window's initial vsync mode.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param vsync `true` for on, `false` for off.
    void set_initial_window_vsync(bool vsync) { this->initial_window_vsync = vsync; }

    /// @brief Sets the window's initial adaptive vsync mode.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param adaptive_vsync `true` for on, `false` for off.
    void set_initial_window_adaptive_vsync(bool adaptive_vsync) {
        this->initial_window_adaptive_vsync = adaptive_vsync;
    }

    /// @brief Sets the window's initial clear color.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param red The red component. [0.0F, 1.0F].
    /// @param green The green component. [0.0F, 1.0F].
    /// @param blue The blue component. [0.0F, 1.0F].
    /// @param alpha The alpha component. [0.0F, 1.0F].
    void set_initial_renderer_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
        this->initial_renderer_clear_color_r = red;
        this->initial_renderer_clear_color_g = green;
        this->initial_renderer_clear_color_b = blue;
        this->initial_renderer_clear_color_a = alpha;
    }

    /// @brief Sets the window's initial attributes from a configuration object.
    ///
    /// @details This will attempt to find the best window configuration that matches the passed configuration and set
    /// that as the initial window configuration. This only sets the size, fullscreen mode, vsync mode, adaptive vsync
    /// mode, and display mode (if fullscreen).
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param config The object containing the initial window configuration.
    /// @todo Implement.
    void set_initial_window_config(WindowConfig config);

    /// @brief Gets the window's current configuration as a configuration object.
    ///
    /// @details This will create a configuration object from the current window configuration. This configuration
    /// object will only encapsulate the size, fullscreen mode, vsync mode, adaptive vsync mode, and display mode (if
    /// fullscreen).
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @return The current window configuration.
    /// @todo Implement.
    WindowConfig get_current_window_config();

    /// @brief Gets the size of the window's draw area in pixels.
    ///
    /// @details This returns the size of the window's draw area, not the size of the window itself, which may be
    /// different. This may be updated by the engine during event handling, and so should be queried each frame.
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @param[out] width Receives the width, in pixels.
    /// @param[out] height Receives the height, in pixels.
    void get_window_size(int *width, int *height) const {
        *width = this->window_w;
        *height = this->window_h;
    }

    /// @brief Sets the vsync mode.
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @param vsync `true` for on, `false` for off.
    /// @param adaptive `true` for on, `false` for off. Ignored if vsync is `false`.
    static void set_vsync(bool vsync, bool adaptive);
};

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_WINDOW_H
