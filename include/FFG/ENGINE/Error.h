//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Error.h
///
/// @brief Defines the Error class.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_ERROR_H
#define FFG_ENGINE_ERROR_H

#include <exception>

namespace FFG::ENGINE {

/// @class Error
///
/// @brief An error that is handled by the engine automatically when thrown.
class Error : public std::exception {
private:
    const char *message;

public:
    /// @brief Creates an Error with the specified message.
    ///
    /// @param message The error message.
    explicit Error(const char *message);

    /// @brief Gets the error message.
    ///
    /// @return The error message.
    [[nodiscard]] const char *get_message() const;
};

/// @brief Creates an error that indicates there was an error with SDL.
///
/// @details The Error returned by this function should be thrown whenever there was an error with any SDL function that
/// sets an error message retrievable by SDL_GetError(). It will automatically get that message and append it to the
/// message passed here in the format "{message}: {sdl_message}".
///
/// @param message The error message to prepend to the SDL error message.
/// @return The Error to throw.
Error SDL_Error(const char *message);

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_ERROR_H
