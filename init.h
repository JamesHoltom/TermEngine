/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef INIT_H
#define INIT_H

#include "sdl_includes.h"
#include "gl_includes.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace term_engine {
  int InitSDL();
  int InitGL();
  int InitGLEW();
  int InitFreeType(FT_Library* library);

  void CleanUpSDL();
  void CleanUpFreeType(const FT_Library& library);
}

#endif // ! INIT_H