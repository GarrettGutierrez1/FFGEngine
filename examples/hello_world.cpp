#define SDL_MAIN_USE_CALLBACKS 1
#include "FFG/ENGINE/Engine.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    // ALLOCATE ENGINE:
    FFG::ENGINE::Engine *engine = nullptr;
    void *mem = SDL_malloc(sizeof(FFG::ENGINE::Engine));
    if (mem == nullptr) {
        return SDL_APP_FAILURE;
    }
    engine = new (mem) FFG::ENGINE::Engine();

    // SET ENGINE METADATA:
    engine->set_app_name("Hello World");
    engine->set_app_version("1.0.0");
    engine->set_app_type("application");

    // SET ENGINE INITIAL CONFIGURATION:
    engine->set_initial_window_title("Hello World");
    engine->set_initial_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);
    engine->set_initial_window_resizeable(false);
    engine->set_initial_window_fullscreen(false);
    engine->set_initial_renderer_clear_color(1.0F, 1.0F, 1.0F, 1.0F);

    return FFG::ENGINE::Engine::AppInit(engine, appstate, argc, argv);
}

SDL_AppResult SDL_AppIterate(void *appstate) { return FFG::ENGINE::Engine::AppIterate(appstate); }

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) { return FFG::ENGINE::Engine::AppEvent(appstate, event); }

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    FFG::ENGINE::Engine *engine = FFG::ENGINE::Engine::AppQuit(appstate, result);

    // DEALLOCATE ENGINE:
    SDL_free(engine);
}
