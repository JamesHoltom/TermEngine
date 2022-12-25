/// @author James Holtom

#ifndef VIEW_H
#define VIEW_H

#include "Character.h"
#include "../utility/GLUtils.h"

namespace rendering {
  /// @brief The default number of rows/columns in the view.
  constexpr glm::uvec2 DEFAULT_DIMENSIONS = glm::uvec2(32, 16);

  /// @brief The position of the view.
  extern glm::vec2 view_position;
  /// @brief The size of the view, in rows/columns.
  extern glm::ivec2 view_size;

  /// @brief Creates the default view.
  void Init();

  /// @brief Destroys all views in the list.
  void CleanUp();

  /// @brief Renders the list of views to the window.
  void Render();

  /**
   * @brief Gets the position of the view on the window.
   * 
   * @returns The position of the view.
   */
  glm::vec2 GetPosition();

  /**
   * @brief Sets the position of the view on the window.
   * 
   * @param[in] position The position to move the view to.
   */
  void SetPosition(const glm::vec2& position);

  /**
   * @brief Gets the size of the view.
   * 
   * @returns The size of the view.
   */
  glm::ivec2 GetSize();

  /**
   * @brief Sets the size of the view, in rows/columns.
   * 
   * @param[in] size The new size of the view.
   */
  void SetSize(const glm::ivec2& size);

  /**
   * @brief Pushes vertex/texture data for the character at the given index to the buffer.
   * 
   * @param[in] index The index of the character to push.
   * @param[in] position The position to render the character to.
   * @param[in] params The character parameters.
   */
  void PushCharacterToBuffer(const size_t& index, const glm::ivec2& position, const objects::CharacterParams& params);

  /// @brief Sets up the character shader.
  void CreateShader();

  /// @brief Destroys the character shader.
  void CleanUpShader();
}

#endif // ! VIEW_H
