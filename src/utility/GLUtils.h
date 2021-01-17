/// @author James Holtom

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
  /// Allows OpenGL to log any errors that occur.
  /**
   * @param[in] source     In which part of OpenGL did the error occur.
   * @param[in] type       The type of error that has occured.
   * @param[in] id         The ID of the error.
   * @param[in] severity   The error's severity.
   * @param[in] length     The length of the error message.
   * @param[in] message    An array pointer to the error message.
   * @param[in] user_param Unused.
   */
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* user_param);
}

#endif // ! GL_UTILS_H
