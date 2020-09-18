/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "../sdl_includes.h"
#include "../gl_includes.h"
#include "../utility/SDLUtils.h"

namespace term_engine::windows {
  constexpr int DEFAULT_WIDTH = 640;
  constexpr int DEFAULT_HEIGHT = 480;

  void Init();
  void CleanUp();

  extern SDL_GLContext context;
  extern SDL::Window window;
}

#endif
