//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Engine.h
///
/// @brief Defines the Engine class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_ENGINE_H
#define FFG_ENGINE_ENGINE_H

#include "FFG/ENGINE/StateManager.h"
#include "FFG/ENGINE/Timer.h"
#include "FFG/ENGINE/Window.h"
#include <SDL3/SDL.h>

namespace FFG::ENGINE {

class State;

/// @class Engine
///
/// @brief The engine that drives the application.
class Engine : public StateManager, public Timer, public Window {
private:
    int limit_fps = 0;
    int limit_fps_off_focus = 0;
    bool wait_event_mode = false;
    bool focus_lost = false;
    bool flag_quit = false;
    bool flag_error = false;
    const char *error_message = nullptr;
    int argc = 0;
    char **argv = nullptr;
    const char *app_name = nullptr;
    const char *app_version = nullptr;
    const char *app_identifier = nullptr;
    const char *app_creator = nullptr;
    const char *app_copyright = nullptr;
    const char *app_url = nullptr;
    const char *app_type = nullptr;
    void init();
    void handle(SDL_Event *event);
    void update();
    void render();
    void exit();
    void _set_main_callback_rate() const;

public:
    Engine() : StateManager(), Timer(), Window() {}

    /// @brief Gets the argc from SDL_AppInit().
    ///
    /// @return The argc value.
    [[nodiscard]] int get_argc() const { return this->argc; }

    /// @brief Gets the argv from SDL_AppInit().
    ///
    /// @return the argv value.
    [[nodiscard]] char **get_argv() const { return this->argv; }

    /// @brief Sets the application name metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_name The application name metadata.
    void set_app_name(const char *app_name) { this->app_name = app_name; }

    /// @brief Sets the application version metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_version The application version metadata.
    void set_app_version(const char *app_version) { this->app_version = app_version; }

    /// @brief Sets the application identifier metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_identifier The application identifier metadata.
    void set_app_identifier(const char *app_identifier) { this->app_identifier = app_identifier; }

    /// @brief Sets the application creator metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_creator The application creator metadata.
    void set_app_creator(const char *app_creator) { this->app_creator = app_creator; }

    /// @brief Sets the application copyright metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_copyright The application copyright metadata.
    void set_app_copyright(const char *app_copyright) { this->app_copyright = app_copyright; }

    /// @brief Sets the application url metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_url The application url metadata.
    void set_app_url(const char *app_url) { this->app_url = app_url; }

    /// @brief Sets the application type metadata.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param app_type The application type metadata.
    void set_app_type(const char *app_type) { this->app_type = app_type; }

    /// @brief Returns `true` if the engine's quit flag has been set by #set_quit().
    ///
    /// @return `true` if the engine's quit flag has been set, otherwise `false`.
    [[nodiscard]] bool is_quit() const { return this->flag_quit; }

    /// @brief Returns `true` if the engine's error flag has been set by #set_error().
    ///
    /// @return `true` if the engine's error flag has been set, otherwise `false`.
    [[nodiscard]] bool is_error() const { return this->flag_error; }

    /// @brief Sets the main callback rate.
    ///
    /// @details The main callback rate is the rate at which update() and render() are called. This may be called before
    /// or after engine initialization. If wait_event_mode is set to `true`, the values for limit_fps and
    /// limit_fps_off_focus will be ignored when the window is both on and off focus.
    ///
    /// @param limit_fps The FPS to limit the framerate to when the window is in focus. `0` for no limit. Default `0`.
    /// @param limit_fps_off_focus The FPS to limit the framerate to when the window is out of focus. `0` for no limit.
    /// Default `0`.
    /// @param wait_event_mode `true` wait on events, otherwise `false`. Default `false`.
    void set_main_callback_rate(int limit_fps = 0, int limit_fps_off_focus = 0, bool wait_event_mode = false);

    /// @brief Sets the engine's quit flag to `true`, quitting the application.
    ///
    /// @details If this is called within a state's State#init(), State#handle(), State#update(), or State#render()
    /// methods, the engine will quit immediately after the function terminates.
    ///
    /// @note This should only be called within a state's State#init(), State#handle(), State#update(), or
    /// State#render() methods. Its behavior is undefined otherwise. Most notably, it should NOT be called within
    /// State#exit().
    void set_quit() { this->flag_quit = true; }

    /// @brief Sets the engine's error flag to `true`, quitting the application with an error. If this is called within
    /// a state's State#init(), State#handle(), State#update(), or State#render() methods, the engine will quit
    /// immediately after the function terminates. If an error handling callback has been set, it will be invoked.
    /// Otherwise the default behavior is to open a message box with the error message if the error ocurred after the
    /// window has been created, and to do nothing with the error otherwise. By convention, the error message should not
    /// end in punctuation.
    ///
    /// @note This should only be called within a state's State#init(), State#handle(), State#update(), or
    /// State#render() methods. Its behavior is undefined otherwise. Most notably, it should NOT be called within
    /// State#exit().
    ///
    /// @param error_message The error's error message.
    void set_error(const char *error_message);

    /// @brief SDL main callback for initializing the application.
    ///
    /// @param engine The engine.
    /// @param appstate appstate from SDL_AppInit().
    /// @param argc argc from SDL_AppInit().
    /// @param argv argv from SDL_AppInit().
    /// @return The return value for SDL_AppInit().
    static SDL_AppResult AppInit(Engine *engine, void **appstate, int argc, char **argv);

    /// @brief SDL main callback for iterating the application.
    ///
    /// @param appstate appstate from SDL_AppInit().
    /// @return The return value for SDL_AppIterate().
    static SDL_AppResult AppIterate(void *appstate);

    /// @brief SDL main callback for handling events.
    ///
    /// @param appstate appstate from SDL_AppInit().
    /// @param event event from SDL_AppEvent().
    /// @return The return value for SDL_AppEvent().
    static SDL_AppResult AppEvent(void *appstate, SDL_Event *event);

    /// @brief SDL main callback for quitting the application.
    ///
    /// @param appstate appstate from SDL_AppQuit().
    /// @param result result from SDL_AppQuit().
    /// @return A pointer to the engine, if needed for deallocation.
    /// @todo Handle the error.
    static Engine *AppQuit(void *appstate, SDL_AppResult result);
};

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_ENGINE_H
