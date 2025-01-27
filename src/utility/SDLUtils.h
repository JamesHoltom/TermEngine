/// @author James Holtom

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>
#include <SDL2/SDL.h>

namespace term_engine::utility {
  /// @brief The flags to initialise SDL with.
  constexpr uint32_t SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;

  /**
   * @brief Initialises SDL.
   * 
   * @returns Was SDL successfully initialised?
   */
  bool InitSDL();

  /// @brief Shuts down SDL.
  void CleanUpSDL();
}

#endif // ! SDL_UTILS_H
