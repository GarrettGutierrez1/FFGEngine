#include <cstddef>

//===--------------------------------------------------------------------------------------------------------------===//
//
// License: http://www.apache.org/licenses/LICENSE-2.0
//
//===--------------------------------------------------------------------------------------------------------------===//
///
/// @file Constants.h
///
/// @brief Defines constants used internally by the FFG engine.
///
//===--------------------------------------------------------------------------------------------------------------===//

#ifndef FFG_ENGINE_CONSTANTS_H
#define FFG_ENGINE_CONSTANTS_H

namespace FFG::ENGINE {

/// @cond
inline constexpr int DEFAULT_SDL_GL_CONTEXT_MAJOR_VERSION = 4;
inline constexpr int DEFAULT_SDL_GL_CONTEXT_MINOR_VERSION = 6;
inline constexpr int DEFAULT_SDL_GL_RED_SIZE = 8;
inline constexpr int DEFAULT_SDL_GL_GREEN_SIZE = 8;
inline constexpr int DEFAULT_SDL_GL_BLUE_SIZE = 8;
inline constexpr int DEFAULT_SDL_GL_ALPHA_SIZE = 8;
inline constexpr int DEFAULT_SDL_GL_DEPTH_SIZE = 24;
inline constexpr int DEFAULT_SDL_GL_STENCIL_SIZE = 8;
inline constexpr int DEFAULT_FFG_WINDOW_SIZE_W = 800;
inline constexpr int DEFAULT_FFG_WINDOW_SIZE_H = 600;
inline constexpr bool DEFAULT_FFG_WINDOW_RESIZEABLE = false;
inline constexpr bool DEFAULT_FFG_WINDOW_FULLSCREEN = false;
inline constexpr bool DEFAULT_FFG_WINDOW_VSYNC = true;
inline constexpr bool DEFAULT_FFG_WINDOW_ADAPTIVE_VSYNC = false;
inline constexpr float DEFAULT_FFG_RENDERER_CLEAR_R = 0.0F;
inline constexpr float DEFAULT_FFG_RENDERER_CLEAR_G = 0.0F;
inline constexpr float DEFAULT_FFG_RENDERER_CLEAR_B = 0.0F;
inline constexpr float DEFAULT_FFG_RENDERER_CLEAR_A = 1.0F;
inline constexpr std::size_t DEFAULT_FFG_MEMORY_ARENA_BLOCK_SIZE = static_cast<std::size_t>(64 * 1024 * 1024);
/// @endcond

} // namespace FFG::ENGINE

#endif // FFG_ENGINE_CONSTANTS_H
