/// @author James Holtom

#ifndef VIEW_H
#define VIEW_H

#include "../objects/Glyph.h"
#include "../utility/GLUtils.h"

namespace term_engine::views {
  /// @brief The default number of rows/columns in the view.
  constexpr glm::uvec2 DEFAULT_DIMENSIONS = glm::uvec2(32, 16);

  /// @brief The ID of the glyph shader program.
  extern GLuint program_id;
  /// @brief The ID of the VAO used to contain the VBO.
  extern GLuint vao_id;
  /// @brief The ID of the VBO used to store the buffer-related data.
  extern GLuint vbo_id;
  /// @brief The position of the view.
  extern glm::vec2 view_position;
  /// @brief The size of the view, in rows/columns.
  extern glm::ivec2 view_size;
  /// @brief The buffer of view data to render to the window.
  extern objects::BufferList data;

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
   * @brief Pushes vertex/texture data for the glyph at the given index to the buffer.
   * 
   * @param[in] index The index of the glyph to push.
   * @param[in] position The position to render the glyph to.
   * @param[in] params The glyph parameters.
   */
  void PushGlyphToBuffer(const size_t& index, const glm::ivec2& position, const objects::GlyphParams& params);

  /// @brief Sets up the OpenGL buffers.
  void CreateBuffers();

  /// @brief Sets up the glyph shader.
  void CreateShader();

  /// @brief Destroys the OpenGL buffers.
  void CleanUpBuffers();

  /// @brief Destroys the glyph shader.
  void CleanUpShader();
}

#endif // ! VIEW_H
