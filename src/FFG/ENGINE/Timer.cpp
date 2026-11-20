//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Timer.cpp
///
/// @brief Implements the Timer class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#include "FFG/ENGINE/Timer.h"

namespace FFG::ENGINE {

/// @cond
void Timer::init() { this->time_last_frame = SDL_GetTicksNS(); }

void Timer::update_timer() {
    this->time_curr_frame = SDL_GetTicksNS();
    this->frame_delta_time =
        static_cast<double>(this->time_curr_frame - this->time_last_frame) / static_cast<double>(SDL_NS_PER_SECOND);
    this->time_last_frame = this->time_curr_frame;
}
/// @endcond

double Timer::time_elapsed(Uint64 start_time) {
    return static_cast<double>(start_time - SDL_GetTicksNS()) / static_cast<double>(SDL_NS_PER_SECOND);
}

} // namespace FFG::ENGINE
