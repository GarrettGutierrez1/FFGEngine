//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Error.cpp
///
/// @brief Implements the Error class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#include "FFG/ENGINE/Error.h"
#include <SDL3/SDL.h>
#include <format>

namespace FFG::ENGINE {

Error::Error(const char *message) : message(message) {}

const char *Error::get_message() const { return message; }

Error SDL_Error(const char *message) {
    const char *sdl_error = SDL_GetError();
    if (sdl_error == nullptr) {
        return Error(message);
    }
    return Error(std::format("{}: {}", message, sdl_error).c_str());
}

} // namespace FFG::ENGINE
