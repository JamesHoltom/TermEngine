/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>

#ifndef _WIN32
#define __stdcall
#define APIENTRY
#endif

namespace GL {
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
}

#endif // ! GL_UTILS_H
