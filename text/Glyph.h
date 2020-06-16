/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_H
#define GLYPH_H

#include "../includes.h"

namespace term_engine {
  typedef struct _Glyph {
    Uint32 character;
    SDL_Color foreground;
    SDL_Color background;

    _Glyph() : character(L'\u0000'), foreground({ 0, 0, 0, 0 }), background({ 0, 0, 0, 0 }) {};
    _Glyph(const Uint32& glyph_char, const SDL_Color& glyph_fg, const SDL_Color& glyph_bg) : character(glyph_char), foreground(glyph_fg), background(glyph_bg) {};
  } Glyph;

  extern Glyph nullGlyph;
}

#endif // ! GLYPH_H