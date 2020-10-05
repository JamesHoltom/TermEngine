/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>

#include <SDL2/SDL.h>

namespace SDL {
  typedef std::remove_pointer<SDL_GLContext>::type SDL_GL_pContext;

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

  struct WindowDestroyer {
    void operator()(SDL_Window* window) const noexcept {
      SDL_DestroyWindow(window);
      window = nullptr;
    }
  };

  struct RendererDestroyer {
    void operator()(SDL_Renderer* renderer) const noexcept {
      SDL_DestroyRenderer(renderer);
      renderer = nullptr;
    }
  };

  struct SurfaceDestroyer {
    void operator()(SDL_Surface* surface) const noexcept {
      SDL_FreeSurface(surface);
      surface = nullptr;
    }
  };

  struct TextureDestroyer {
    void operator()(SDL_Texture* texture) const noexcept {
      SDL_DestroyTexture(texture);
      texture = nullptr;
    }
  };

  typedef std::unique_ptr<SDL_Window, WindowDestroyer> Window;
  typedef std::unique_ptr<SDL_Renderer, RendererDestroyer> Renderer;
  typedef std::unique_ptr<SDL_Surface, SurfaceDestroyer> Surface;
  typedef std::unique_ptr<SDL_Texture, TextureDestroyer> Texture;
}

#endif
