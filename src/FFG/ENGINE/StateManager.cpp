//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file StateManager.cpp
///
/// @brief Implements the StateManager class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#include "FFG/ENGINE/StateManager.h"
#include "FFG/ENGINE/State.h"

namespace FFG::ENGINE {

/// @cond
void StateManager::init() {
    this->next_state = nullptr;
    if (this->initial_state != nullptr) {
        this->current_state = this->initial_state;
    }
    if (this->current_state != nullptr) {
        this->current_state->init();
    }
}

void StateManager::exit() {
    this->next_state = nullptr;
    if (this->current_state != nullptr) {
        this->current_state->exit();
        this->current_state = nullptr;
    }
}

void StateManager::handle(SDL_Event *event) {
    if (this->current_state != nullptr) {
        this->current_state->handle(event);
    }
}

void StateManager::update() {
    if (this->current_state != nullptr) {
        this->current_state->update();
    }
}

void StateManager::render() {
    if (this->current_state != nullptr) {
        this->current_state->render();
    }
}

void StateManager::swap_states() {
    if (this->next_state != nullptr) {
        if (this->current_state != nullptr) {
            this->current_state->exit();
        }
        this->current_state = this->next_state;
        this->next_state = nullptr;
        if (this->current_state != nullptr) {
            this->current_state->init();
        }
    }
}
/// @endcond

} // namespace FFG::ENGINE
