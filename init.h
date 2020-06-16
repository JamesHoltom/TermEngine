/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef INIT_H
#define INIT_H

#include "sdl_includes.h"
#include "gl_includes.h"
#include "includes.h"

namespace term_engine {
  int InitSDL();
  int InitGL();
  int InitGLEW();

  void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

  void Shutdown();
}

#endif // ! INIT_H