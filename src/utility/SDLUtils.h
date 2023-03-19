/// @author James Holtom

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>
#include <SDL2/SDL.h>

namespace term_engine::utility {
  /// @brief The flags to initialise SDL with.
  constexpr uint32_t SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;

  /// @brief Wraps _SDL_Window_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> SDLWindow;
  /// @brief Wraps _SDL_Renderer_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> SDLRenderer;
  /// @brief Wraps _SDL_Surface_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> SDLSurface;
  /// @brief Wraps _SDL_Texture_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> SDLTexture;

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
