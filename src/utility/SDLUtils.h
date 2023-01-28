/// @author James Holtom

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>
#include <SDL2/SDL.h>

namespace term_engine::utility {
  /// @brief The flags to initialise SDL with.
  constexpr int SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;

  struct WindowDestroyer;
  struct RendererDestroyer;
  struct SurfaceDestroyer;
  struct TextureDestroyer;

  /// @brief Wraps _SDL_Window_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Window, WindowDestroyer> SDLWindow;
  /// @brief Wraps _SDL_Renderer_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Renderer, RendererDestroyer> SDLRenderer;
  /// @brief Wraps _SDL_Surface_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Surface, SurfaceDestroyer> SDLSurface;
  /// @brief Wraps _SDL_Texture_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Texture, TextureDestroyer> SDLTexture;

  /// @brief Smart pointer destructor used to destroy a _SDL_Window_.
  struct WindowDestroyer {
    /**
     * @brief Operator for calling the destructor like a function.
     * 
     * @param[in,out] window The window to destroy.
     */
    void operator()(SDL_Window* window) const noexcept
    {
      SDL_DestroyWindow(window);
      window = nullptr;
    }
  };

  /// @brief Smart pointer destructor used to destroy a _SDL_Renderer_.
  struct RendererDestroyer {
    /**
     * @brief Operator for calling the destructor like a function.
     * 
     * @param[in,out] renderer The renderer to destroy.
     */
    void operator()(SDL_Renderer* renderer) const noexcept
    {
      SDL_DestroyRenderer(renderer);
      renderer = nullptr;
    }
  };

  /// @brief Smart pointer destructor used to destroy a _SDL_Surface_.
  struct SurfaceDestroyer {
    /**
     * @brief Operator for calling the destructor like a function.
     * 
     * @param[in,out] surface The surface to destroy.
     */
    void operator()(SDL_Surface* surface) const noexcept
    {
      SDL_FreeSurface(surface);
      surface = nullptr;
    }
  };

  /// @brief Smart pointer destructor used to destroy a _SDL_Texture_.
  struct TextureDestroyer {
    /**
     * @brief Operator for calling the destructor like a function.
     * 
     * @param[in,out] texture The texture to destroy.
     */
    void operator()(SDL_Texture* texture) const noexcept
    {
      SDL_DestroyTexture(texture);
      texture = nullptr;
    }
  };

  /**
   * @brief Initialises SDL.
   * 
   * @returns Was SDL successfully initialised?
   */
  int InitSDL();

  /// @brief Shuts down SDL.
  void CleanUpSDL();
}

#endif // ! SDL_UTILS_H
