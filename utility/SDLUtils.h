/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <memory>

#include <SDL2/SDL_render.h>

namespace SDL {
  struct WindowDestroyer {
    void operator()(SDL_Window* window) const noexcept {
      SDL_DestroyWindow(window);
    }
  };

  struct RendererDestroyer {
    void operator()(SDL_Renderer* renderer) const noexcept {
      SDL_DestroyRenderer(renderer);
    }
  };

  struct SurfaceDestroyer {
    void operator()(SDL_Surface* surface) const noexcept {
      SDL_FreeSurface(surface);
    }
  };

  struct TextureDestroyer {
    void operator()(SDL_Texture* texture) const noexcept {
      SDL_DestroyTexture(texture);
    }
  };

  typedef std::unique_ptr<SDL_Window, WindowDestroyer> Window;
  typedef std::unique_ptr<SDL_Renderer, RendererDestroyer> Renderer;
  typedef std::unique_ptr<SDL_Surface, SurfaceDestroyer> Surface;
  typedef std::unique_ptr<SDL_Texture, TextureDestroyer> Texture;
}

#endif
