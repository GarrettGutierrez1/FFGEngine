//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file StateManager.h
///
/// @brief Defines the StateManager class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_STATEMANAGER_H
#define FFG_ENGINE_STATEMANAGER_H

#include <SDL3/SDL.h>

namespace FFG::ENGINE {

class State;

/// @class StateManager
///
/// @brief An interface for the #Engine's state management system.
class StateManager {
private:
    State *current_state = nullptr;
    State *next_state = nullptr;
    State *initial_state = nullptr;

protected:
    /// @cond
    void init();
    void exit();
    void handle(SDL_Event *event);
    void update();
    void render();
    void swap_states();
    /// @endcond

public:
    /// @brief Sets the initial state.
    ///
    /// @note This should only be called before engine initialization. It has has no effect otherwise.
    ///
    /// @param state The initial state.
    void set_initial_state(State *state) { this->initial_state = state; }

    /// @brief Sets the next state.
    ///
    /// @details This is called within a state's State#handle(), State#update(), or State#render() methods to flag the
    /// next state. When a state calls this within any of the aforementioned valid calling functions, the current state
    /// will be swapped with the next state as soon as the calling function terminates. When this swap occurs, the
    /// current state's State#exit() method is called, followed by the next state's State#init() method.
    ///
    /// Note the immediacy with which states are swapped and the fact that they can be swapped between calls to
    /// State#handle(), State#update(), and State#render(). For example, if there have been 2 events since the last
    /// render, and the current state calls set_next_state() while handling the first event, a state swap will occur
    /// between the events and the second event will be dispatched to and be handled by the next state even though it
    /// ocurred during the previous state. Similarly, if a state calls set_next_state() while updating, the states will
    /// be swapped and the next state will immediately be rendered before processing its first update.
    ///
    /// @warning This should only be called within a state's State#handle(), State#update(), or State#render() methods.
    /// Its behavior is undefined otherwise. Most notably, it should NOT be called within State#init() or State#exit().
    ///
    /// @param state The next state.
    void set_next_state(State *state) { this->next_state = state; }
};

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_STATEMANAGER_H
