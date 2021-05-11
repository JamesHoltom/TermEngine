/// @author James Holtom

#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  /// The location of the vertex GLSL file for the shader.
  constexpr char GLSL_VERTEX_FILE[] = "resources/shaders/core/glyph.vs";
  /// The location of the geometry GLSL file for the shader.
  constexpr char GLSL_GEOMETRY_FILE[] = "resources/shaders/core/glyph.gs";
  /// The location of the fragment GLSL file for the shader.
  constexpr char GLSL_FRAGMENT_FILE[] = "resources/shaders/core/glyph.fs";

  /// Initialises the shader program and prepares it for use.
  /**
   * @returns If the shader program was successfully set up.
   */
  int Init();

  /// Destroys the shader program.
  void CleanUp();

  /// Compiles a GLSL file into a shader, ready to link to a program.
  /**
   * @param[in] filename The file to compile.
   * @param[in] type     The type of shader to compile.
   * @see [OpenGL shader stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
   * @returns The ID of the compiled shader, or 0 if it failed to compile.
   */
  GLuint AddShaderFile(const std::string& filename, const GLenum& type);

  /// Logs the build log of the GLSL program.
  void PrintProgramLog();

  /// Logs the compilation/linking log of the given shader stage.
  /**
   * @param[in] stage_id The ID of the shader to log.
   */
  void PrintStageLog(const GLuint& stage_id);

  /// The ID of the shader program.
  extern GLuint program_id;
}

#endif // ! SHADER_H
