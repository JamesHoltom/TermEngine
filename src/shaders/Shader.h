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
  constexpr auto GLSL_VERTEX_FILE = "resources/shaders/core/glyph.vs";
  constexpr auto GLSL_GEOMETRY_FILE = "resources/shaders/core/glyph.gs";
  constexpr auto GLSL_FRAGMENT_FILE = "resources/shaders/core/glyph.fs";

  int Init();
  void CleanUp();

  GLuint AddShaderFile(const std::string& filename, const GLenum& type);

  /// Logs the build log of the GLSL program.
  void PrintProgramLog();

  /// Logs the compilation/linking log of the given shader stage.
  /**
   * @param[in] stage_id The ID of the shader to log.
   */
  void PrintStageLog(const GLuint& stage_id);

  extern GLuint program_id;
}

#endif // ! SHADER_H
