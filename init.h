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

#ifndef WIN32
#define __stdcall
#define APIENTRY
#endif

namespace term_engine {
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

  int InitSDL();
  int InitGL();
  int InitGLEW();

  void Shutdown();
}

#endif // ! INIT_H