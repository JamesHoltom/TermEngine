/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <string>
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

namespace term_engine::debug {
  void LogKeyboardEvents(SDL_Event& event);
  void LogWindowEvents(SDL_Event& event);
}

#endif
