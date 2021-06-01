/// @author James Holtom

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  using ShaderInitialisationPair = std::pair<GLenum, std::string>;

  /// Creates an OpenGL shader program.
  /***
   * @returns The program ID.
   */
  GLuint CreateProgram();

  /// Compiles a GLSL file into a shader, ready to link to a program.
  /**
   * @param[in] program_id The ID of the program to build the shader stage for.
   * @param[in] shader The shader stage enum & filename to compile.
   * @see [OpenGL shader stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
   * @returns The ID of the compiled shader, or 0 if it failed to compile.
   */
  GLint AddShaderFile(const GLuint& program_id, const ShaderInitialisationPair& shader);

  /// Builds & links the compiled shader stages of a program.
  /**
   * @param[in] program_id The ID of the shader program to build.
   * @returns Whether the program was successfully built.
   */
  GLint LinkProgram(const GLuint& program_id);

  /// Logs the build log of the GLSL program.
  /**
   * @param[in] program_id The ID of the program to log.
   */
  void PrintProgramLog(const GLuint& program_id);

  /// Logs the compilation/linking log of the given shader stage.
  /**
   * @param[in] stage_id The ID of the shader stage to log.
   * @see [OpenGL shader stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
   */
  void PrintStageLog(const GLuint& stage_id);
}

#endif // ! SHADER_H
