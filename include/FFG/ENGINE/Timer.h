//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Timer.h
///
/// @brief Defines the Timer class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_TIMER_H
#define FFG_ENGINE_TIMER_H

#include <SDL3/SDL.h>

namespace FFG::ENGINE {

/// @class Timer
///
/// @brief An interface for the #Engine's time management system.
class Timer {
private:
    Uint64 time_last_frame = 0.0;
    Uint64 time_curr_frame = 0.0;
    double frame_delta_time = 0.0F;

protected:
    /// @cond
    void init();
    void update_timer();
    /// @endcond

public:
    /// @brief Gets the time in seconds between the processing of the last 2 frames.
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @return The delta time in seconds.
    [[nodiscard]] double get_delta_time() const { return this->frame_delta_time; }

    /// @brief Gets the current nanosecond tick.
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @return The current nanosecond tick.
    [[nodiscard]] static Uint64 get_tick_ns() { return SDL_GetTicksNS(); }

    /// @brief Gets the time in seconds that has elapsed since the start time.
    ///
    /// @details The start time should be a value returned by #get_tick_ns().
    ///
    /// @warning This should only be called after engine initialization. Its behavior is undefined otherwise.
    ///
    /// @param start_time The start time (as a nanosecond tick).
    /// @return The elapsed time in seconds.
    static double time_elapsed(Uint64 start_time);
};

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_TIMER_H
