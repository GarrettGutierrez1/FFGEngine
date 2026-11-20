//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Window.cpp
///
/// @brief Implements the Window class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#include "FFG/ENGINE/Window.h"
#include "FFG/ENGINE/Constants.h"
#include "FFG/ENGINE/Error.h"

namespace FFG::ENGINE {

/// @cond
void Window::init() {
    // SET OPENGL ATTRIBUTES:
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL context profile mask attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DEFAULT_SDL_GL_CONTEXT_MAJOR_VERSION)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL context major version attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DEFAULT_SDL_GL_CONTEXT_MINOR_VERSION)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL context minor version attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_RED_SIZE, DEFAULT_SDL_GL_RED_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL red buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, DEFAULT_SDL_GL_GREEN_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL green buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, DEFAULT_SDL_GL_BLUE_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL blue buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, DEFAULT_SDL_GL_ALPHA_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL alpha buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEFAULT_SDL_GL_DEPTH_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL depth buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, DEFAULT_SDL_GL_STENCIL_SIZE)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL stencil buffer size attribute");
    }
    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set OpenGL double buffer attribute");
    }

    // CREATE SDL WINDOW:
    SDL_Window *window =
        SDL_CreateWindow(this->initial_window_title, this->initial_window_size_w, this->initial_window_size_h,
                         SDL_WINDOW_OPENGL | (this->initial_window_resizeable ? SDL_WINDOW_RESIZABLE : 0));
    if (window == nullptr) {
        throw FFG::ENGINE::SDL_Error("SDL failed to create window");
    }
    this->window = window;
    if (this->initial_window_fullscreen && !SDL_SetWindowFullscreen(this->window, true)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set window fullscreen");
    }
    SDL_SyncWindow(this->window);
    if (!SDL_GetWindowSizeInPixels(this->window, &this->window_w, &this->window_h)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to get window size in pixels");
    }

    // CREATE OPENGL CONTEXT:
    SDL_GLContext glContext = SDL_GL_CreateContext(this->window);
    if (glContext == nullptr) {
        throw FFG::ENGINE::SDL_Error("SDL failed to create OpenGL context");
    }
    this->glContext = glContext;
    if (!SDL_GL_MakeCurrent(window, this->glContext)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set current context");
    }

    // USE GLAD TO LOAD OPENGL:
    int version = gladLoadGL(static_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
    if (version == 0) {
        throw FFG::ENGINE::Error("GLAD failed to load OpenGL");
    }
    if (GLAD_VERSION_MAJOR(version) != DEFAULT_SDL_GL_CONTEXT_MAJOR_VERSION ||
        GLAD_VERSION_MINOR(version) != DEFAULT_SDL_GL_CONTEXT_MINOR_VERSION) {
        throw FFG::ENGINE::Error("GLAD failed to load correct OpenGL version");
    }

    // SET INITIAL VSYNC:
    FFG::ENGINE::Window::set_vsync(this->initial_window_vsync, this->initial_window_adaptive_vsync);

    // SET INITIAL CLEAR COLOR:
    glClearColor(this->initial_renderer_clear_color_r, this->initial_renderer_clear_color_g,
                 this->initial_renderer_clear_color_b, this->initial_renderer_clear_color_a);
}

void Window::exit() {
    // DESTROY OPENGL CONTEX:
    if (this->glContext != nullptr) {
        SDL_GL_DestroyContext(this->glContext);
        this->glContext = nullptr;
    }

    // DESTROY SDL WINDOW:
    if (this->window != nullptr) {
        SDL_DestroyWindow(this->window);
        this->window = nullptr;
    }
}

void Window::update_window_size(SDL_Event *event) {
    this->window_w = event->window.data1;
    this->window_h = event->window.data2;
    glViewport(0, 0, this->window_w, this->window_h);
}

void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

void Window::swap() {
    if (!SDL_GL_SwapWindow(this->window)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to swap the window buffers");
    }
}
/// @endcond

void Window::set_vsync(bool vsync, bool adaptive) {
    if (vsync) {
        if (adaptive) {
            if (!SDL_GL_SetSwapInterval(-1) && !SDL_GL_SetSwapInterval(1)) {
                throw FFG::ENGINE::SDL_Error("SDL failed to enable adaptive vsync");
            }
        } else if (!SDL_GL_SetSwapInterval(1)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to enable vsync");
        }
    } else if (!SDL_GL_SetSwapInterval(0)) {
        throw FFG::ENGINE::SDL_Error("SDL failed to disable vsync");
    }
}

} // namespace FFG::ENGINE
