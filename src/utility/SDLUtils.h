/// @author James Holtom

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>
#include <SDL2/SDL.h>

namespace SDL {
  namespace color_mask {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    constexpr Uint32 rMask = 0xff000000;
    constexpr Uint32 gMask = 0x00ff0000;
    constexpr Uint32 bMask = 0x0000ff00;
    constexpr Uint32 aMask = 0x000000ff;
#else
    constexpr Uint32 rMask = 0x000000ff;
    constexpr Uint32 gMask = 0x0000ff00;
    constexpr Uint32 bMask = 0x00ff0000;
    constexpr Uint32 aMask = 0xff000000;
#endif
  }

  struct WindowDestroyer;
  struct RendererDestroyer;
  struct SurfaceDestroyer;
  struct TextureDestroyer;

  /// Wraps _SDL_Window_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Window, WindowDestroyer> Window;
  /// Wraps _SDL_Renderer_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Renderer, RendererDestroyer> Renderer;
  /// Wraps _SDL_Surface_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Surface, SurfaceDestroyer> Surface;
  /// Wraps _SDL_Texture_ with a smart pointer to allow automatic cleanup.
  typedef std::unique_ptr<SDL_Texture, TextureDestroyer> Texture;

  /// Smart pointer destructor used to destroy a _SDL_Window_.
  struct WindowDestroyer {
    /// @param[in,out] window The window to destroy.
    void operator()(SDL_Window* window) const noexcept
    {
      SDL_DestroyWindow(window);
      window = nullptr;
    }
  };

  /// Smart pointer destructor used to destroy a _SDL_Renderer_.
  struct RendererDestroyer {
    /// @param[in,out] renderer The renderer to destroy.
    void operator()(SDL_Renderer* renderer) const noexcept
    {
      SDL_DestroyRenderer(renderer);
      renderer = nullptr;
    }
  };

  /// Smart pointer destructor used to destroy a _SDL_Surface_.
  struct SurfaceDestroyer {
    /// @param[in,out] surface The surface to destroy.
    void operator()(SDL_Surface* surface) const noexcept
    {
      SDL_FreeSurface(surface);
      surface = nullptr;
    }
  };

  /// Smart pointer destructor used to destroy a _SDL_Texture_.
  struct TextureDestroyer {
    /// @param[in,out] texture The texture to destroy.
    void operator()(SDL_Texture* texture) const noexcept
    {
      SDL_DestroyTexture(texture);
      texture = nullptr;
    }
  };

  /// Initialises SDL.
  /**
   * @returns Was SDL successfully initialised?
   */
  int InitSDL();

  /// Shuts down SDL.
  void CleanUpSDL();
}

#endif // ! SDL_UTILS_H
