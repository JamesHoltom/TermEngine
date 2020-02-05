/*
 * Copyright 2019
 * James Holtom
 * MIT License
 */

#ifndef UTIL_GLYPH_H
#define UTIL_GLYPH_H

#include <SDL.h>

namespace term_engine::utilities {
  typedef struct {
    Uint32 character;
    SDL_Color foreground;
    SDL_Color background;
  } Glyph;

  extern Glyph nullGlyph;
}

#endif
