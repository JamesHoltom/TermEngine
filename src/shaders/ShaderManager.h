/// @author James Holtom

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <initializer_list>
#include <string>

#include "Shader.h"
#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  /// Stores a list of shaders.
  extern ShaderList shader_list;

  /// Creates the default shader for rendering glyphs.
  void InitGlyphShader();

  /// Checks if the shader with the given name exists in the list.
  /**
   * @param[in] name The shader name to check.
   * @returns If the shader exists in the list.
   */
  bool ShaderExists(const std::string& name);

  /// Returns the shader with the given name.
  /**
   * @param[in] name The name of the shader to get.
   * @returns A pointer to the shader, or `nullptr` if it was not found.
   */
  ShaderPtr GetShader(const std::string& name);

  /// Adds a new shader to the list.
  /**
   * @param[in] name The name of the new shader.
   * @returns A pointer to the newly created shader.
   */
  ShaderPtr AddShader(const std::string& name);

  /// Removes a shader from the list.
  /**
   * @param[in] name The name of the shader to remove.
   */
  void RemoveShader(const std::string& name);

  /// Removes all shaders from the list.
  void CleanUpShaders();

  /// Prints the number of references to all shaders in the list.
  /**
   * Each count also includes the list's reference.
   */
  void GetPointerUsage();
}

#endif // ! SHADER_MANAGER_H
