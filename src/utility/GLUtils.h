/// @author James Holtom

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <string>
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

namespace term_engine::utility {
  /// @brief The major version of OpenGL to request.
  constexpr int MAJOR_VERSION = 4;
  /// @brief The minor version of OpenGL to request.
  constexpr int MINOR_VERSION = 6;

  /// @brief Used to return the results of compiling a shader stage/program.
  typedef std::pair<uint32_t, uint8_t> ShaderProcessResult;

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
  void GLAPIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message, const void* user_param);

  /// @brief Prepares OpenGL for use.
  void InitGL();

  /**
   * @brief Initialises GLEW.
   * @details GLEW needs to be called *after* the window has been initialised.
   *
   * @returns Was GLEW successfully initialised?
   */
  bool InitGLEW();

  /**
   * @brief Logs the build log of the GLSL program.
   * 
   * @param[in] program_id The ID of the program to log.
   */
  void PrintProgramLog(uint32_t program_id);

  /**
   * @brief Logs the compilation/linking log of the given shader stage.
   * 
   * @param[in] stage_id The ID of the shader stage to log.
   * @see [OpenGL shader stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
   */
  void PrintStageLog(uint32_t stage_id);

  /**
   * @brief Links a shader program with the given ID with its attached stages.
   * 
   * @param[in] program_id The shader program ID to link.
   * @returns If the program was successfully linked.
   */
  ShaderProcessResult LinkShaderProgram(uint32_t program_id);

  /**
   * @brief Compiles the shader stage with the given GLSL code and type.
   * 
   * @param[in] glsl_code The GLSL code to compile.
   * @param[in] type      The type of shader stage being compiled.
   * @returns If the shader stage was successfully compiled.
   */
  ShaderProcessResult CompileShaderStage(const std::string& glsl_code, uint32_t type);

  /**
   * @brief Returns the string name for the given shader stage type.
   * 
   * @param[in] type The shader stage type.
   * @returns The string name.
   */
  std::string GetShaderTypeName(uint32_t type);
}

#endif // ! GL_UTILS_H
