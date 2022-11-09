/// @author James Holtom

#ifndef UNIFORM_H
#define UNIFORM_H

#include "../utility/GLUtils.h"

namespace term_engine::data {
  struct UniformData;

  /// @brief The ID of the Uniform Buffer Object (UBO), that stores the uniforms used in the shaders.
  extern GLuint ubo_id;
  /// @brief The uniform data held in memory.
  extern UniformData ubo_data;

  /// @brief Represents the internal structure of the Uniform Buffer Object.
  struct UniformData {
    /// @brief The projection matrix, that maps the window's coordinates (i.e. 0 to _window width_) to OpenGL's coordinates (i.e. -1 to 1).
    glm::mat4 projection_;
    /// @brief The position of the character set in the window.
    glm::vec2 set_position_;
  };

  /// @brief Initialises the Uniform Buffer Object.
  void Init();

  /// @brief Deletes the Uniform Buffer Object.
  void CleanUp();

  /**
   * @brief Updates the projection matrix in the UBO to the given size.
   * 
   * @param[in] size The size of the projection.
   */
  void SetProjection(const glm::ivec2& size);

  /**
   * @brief Returns the position of the character set on the screen.
   * 
   * @returns The position of the character set.
   */
  glm::vec2 GetPosition();

  /**
   * @brief Sets the position of the character set on the screen.
   * 
   * @param[in] position The position to move the set to.
   */
  void SetPosition(const glm::vec2& position);
}

#endif // ! UNIFORM_H
