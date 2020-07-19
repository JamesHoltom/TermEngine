/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SDL_INCLUDES_H
#define SDL_INCLUDES_H

#include <memory>

#include <SDL2/SDL.h>

namespace term_engine {
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

  // Delete methods for smart pointers.
  struct SDL_Deleter {
    void operator() (SDL_Window* win) const {
      SDL_DestroyWindow(win);
      win = nullptr;
    }

    void operator() (SDL_Renderer* rend) const {
      SDL_DestroyRenderer(rend);
      rend = nullptr;
    }

    void operator() (SDL_Texture* tex) const {
      SDL_DestroyTexture(tex);
      tex = nullptr;
    }

    void operator() (SDL_Surface* surf) const {
      SDL_FreeSurface(surf);
      surf = nullptr;
    }
  };
}

#endif // ! SDL_UTILITIES_H
