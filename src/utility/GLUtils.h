/// @author James Holtom

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_opengl.h>

#ifndef _WIN32
#define __stdcall
#define APIENTRY
#endif

namespace GL {
  /**
   * @brief Allows OpenGL to log any errors that occur.
   * 
   * @param[in] source     In which part of OpenGL did the error occur.
   * @param[in] type       The type of error that has occured.
   * @param[in] id         The ID of the error.
   * @param[in] severity   The error's severity.
   * @param[in] length     The length of the error message.
   * @param[in] message    An array pointer to the error message.
   * @param[in] user_param Unused.
   */
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* user_param);

  /**
   * @brief Prepares OpenGL for use. Do not call this function! Use `Init()` instead.
   * 
   * @returns Was OpenGL successfully initialised?
   */
  int InitGL();

  /**
   * @brief Initialises GLEW. Do not call this function! Use `Init()` instead.
   * @details GLEW needs to be called *after* the window has been initialised.
   *
   * @returns Was GLEW successfully initialised?
   */
  int InitGLEW();
}

#endif // ! GL_UTILS_H
