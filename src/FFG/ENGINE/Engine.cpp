//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Engine.cpp
///
/// @brief Implements the Engine class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#include "FFG/ENGINE/Engine.h"
#include "FFG/ENGINE/Error.h"
#include "FFG/ENGINE/State.h"
#include <SDL3/SDL_system.h>
#include <string>

namespace FFG::ENGINE {

void Engine::init() {
    try {
        // SET APPLICATION METADATA:
        if ((this->app_name != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, this->app_name)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (name)");
        }
        if ((this->app_version != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, this->app_version)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (version)");
        }
        if ((this->app_identifier != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, this->app_identifier)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (identifier)");
        }
        if ((this->app_creator != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, this->app_creator)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (creator)");
        }
        if ((this->app_copyright != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, this->app_copyright)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (copyright)");
        }
        if ((this->app_url != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, this->app_url)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (URL)");
        }
        if ((this->app_type != nullptr) &&
            !SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, this->app_type)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set application metadata (type)");
        }
        this->_set_main_callback_rate();

        // INITIALIZE SDL:
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)) {
            throw FFG::ENGINE::SDL_Error("SDL failed to initialize");
        }

        // INITIALIZE SUPERS:
        this->Window::init();
        this->StateManager::init();
        this->Timer::init();
    } catch (const FFG::ENGINE::Error &e) {
        this->set_error(e.get_message());
        return;
    }
}

void Engine::handle(SDL_Event *event) {
    try {
        switch (event->type) {
        case SDL_EVENT_QUIT:
            this->set_quit();
            return;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            this->Window::update_window_size(event);
            return;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            if ((this->limit_fps_off_focus != 0) && !this->wait_event_mode &&
                !SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(this->limit_fps_off_focus).c_str())) {
                throw FFG::ENGINE::SDL_Error("SDL failed to set main callback rate hint");
            }
            this->focus_lost = true;
            return;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            this->_set_main_callback_rate();
            this->focus_lost = false;
            return;
        default:
            break;
        }
        this->StateManager::handle(event);
        if (this->flag_error) {
            throw FFG::ENGINE::Error(this->error_message);
        }
        this->StateManager::swap_states();
    } catch (const FFG::ENGINE::Error &e) {
        this->set_error(e.get_message());
        return;
    }
}

void Engine::update() {
    try {
        this->StateManager::update();
        if (this->flag_error) {
            throw FFG::ENGINE::Error(this->error_message);
        }
        this->StateManager::swap_states();
        if (this->flag_error) {
            throw FFG::ENGINE::Error(this->error_message);
        }
    } catch (const FFG::ENGINE::Error &e) {
        this->set_error(e.get_message());
        return;
    }
}

void Engine::render() {
    try {
        FFG::ENGINE::Engine::clear();
        this->StateManager::render();
        if (this->flag_error) {
            throw FFG::ENGINE::Error(this->error_message);
        }
        this->Window::swap();
        this->Timer::update_timer();
    } catch (const FFG::ENGINE::Error &e) {
        this->set_error(e.get_message());
        return;
    }
}

void Engine::exit() {
    // DEINITIALIZE SUPERS:
    this->StateManager::exit();
    this->Window::exit();

    // DEINITIALIZE SDL:
    SDL_Quit();

    // RESET FLAGS:
    this->flag_quit = false;
    this->flag_error = false;
    this->error_message = nullptr;
}

void Engine::_set_main_callback_rate() const {
    if (this->wait_event_mode) {
        if (!SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "waitevent")) {
            throw FFG::ENGINE::SDL_Error("SDL failed to set main callback rate hint");
        }
    } else if (!SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(this->limit_fps).c_str())) {
        throw FFG::ENGINE::SDL_Error("SDL failed to set main callback rate hint");
    }
}

void Engine::set_main_callback_rate(int limit_fps, int limit_fps_off_focus, bool wait_event_mode) {
    this->limit_fps = limit_fps;
    this->limit_fps_off_focus = limit_fps_off_focus;
    this->wait_event_mode = wait_event_mode;
    if (!this->focus_lost) {
        this->_set_main_callback_rate();
    }
}

void Engine::set_error(const char *error_message) {
    if (this->flag_error) {
        return;
    }
    this->flag_error = true;
    this->flag_quit = true;
    this->error_message = error_message;
}

SDL_AppResult Engine::AppInit(Engine *engine, void **appstate, int argc, char **argv) {
    *appstate = engine;
    engine->argc = argc;
    engine->argv = argv;
    engine->init();
    if (engine->flag_error) {
        return SDL_APP_FAILURE;
    }
    if (engine->flag_quit) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult Engine::AppIterate(void *appstate) {
    auto *engine = static_cast<Engine *>(appstate);
    if (engine == nullptr) {
        return SDL_APP_FAILURE;
    }
    engine->update();
    if (engine->flag_error) {
        return SDL_APP_FAILURE;
    }
    if (engine->flag_quit) {
        return SDL_APP_SUCCESS;
    }
    engine->render();
    if (engine->flag_error) {
        return SDL_APP_FAILURE;
    }
    if (engine->flag_quit) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult Engine::AppEvent(void *appstate, SDL_Event *event) {
    auto *engine = static_cast<Engine *>(appstate);
    if (engine == nullptr) {
        return SDL_APP_FAILURE;
    }
    engine->handle(event);
    if (engine->flag_error) {
        return SDL_APP_FAILURE;
    }
    if (engine->flag_quit) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

Engine *Engine::AppQuit(void *appstate, SDL_AppResult result) {
    auto *engine = static_cast<Engine *>(appstate);
    if (engine == nullptr) {
        return nullptr;
    }
    if (result == SDL_APP_FAILURE) {
        // TODO: Handle the error.
        // SDL has function for creating message boxes, look into them.
    }
    engine->exit();
    return engine;
}

} // namespace FFG::ENGINE
