/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "../gl_includes.h"

#ifndef WIN32
#define __stdcall
#define APIENTRY
#endif

namespace GL {
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}

#endif // !GL_UTILS_H
