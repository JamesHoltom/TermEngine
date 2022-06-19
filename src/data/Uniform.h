/// @author James Holtom

#ifndef UNIFORM_H
#define UNIFORM_H

#include "../utility/GLUtils.h"

namespace term_engine::data {
  struct UniformData;

  /// The ID of the Uniform Buffer Object (UBO), that stores the uniforms used in the shaders.
  extern GLuint ubo_id;
  /// The uniform data held in memory.
  extern UniformData ubo_data;

  /// Represents the internal structure of the Uniform Buffer Object.
  struct UniformData {
    /// The projection matrix, that maps the window's coordinates (i.e. 0 to _window width_) to OpenGL's coordinates (i.e. -1 to 1).
    glm::mat4 projection_;
    /// The position of the glyph set in the window.
    glm::vec2 set_position_;
  };

  /// Initialises the Uniform Buffer Object.
  void Init();

  /// Deletes the Uniform Buffer Object.
  void CleanUp();

  /// Updates the projection matrix in the UBO to the window's current size.
  void SetProjection(const glm::ivec2& window_size);

  /// Returns the position of the glyph set on the screen.
  /**
   * @returns The position of the glyph set.
   */
  glm::vec2 GetPosition();

  /// Sets the position of the glyph set on the screen.
  /**
   * @param[in] position The position to move the set to.
   */
  void SetPosition(const glm::vec2& position);
}

#endif // ! UNIFORM_H
