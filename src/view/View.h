/// @author James Holtom

#ifndef VIEW_H
#define VIEW_H

#include "../utility/Glyph.h"
#include "../utility/GLUtils.h"

namespace term_engine::views {
  /// The location of the vertex GLSL file for the glyph shader.
  constexpr char GLYPH_VERTEX_FILE[] = "resources/shaders/core/glyph.vert";
  /// The location of the geometry GLSL file for the glyph shader.
  constexpr char GLYPH_GEOMETRY_FILE[] = "resources/shaders/core/glyph.geom";
  /// The location of the fragment GLSL file for the glyph shader.
  constexpr char GLYPH_FRAGMENT_FILE[] = "resources/shaders/core/glyph.frag";
  /// The default number of rows/columns in the view.
  constexpr glm::uvec2 DEFAULT_DIMENSIONS = glm::uvec2(32, 16);

  /// Creates the default view.
  void Init();

  /// Destroys all views in the list.
  void CleanUp();

  /// Renders the list of views to the window.
  void Render();

  /// Sets the position of the view on the window.
  /**
   * @param[in] position The position to move the view to.
   */
  void SetPosition(const glm::vec2& position);

  /// Sets the size of the view, in rows/columns.
  /**
   * @param[in] size The new size of the view.
   */
  void SetSize(const glm::ivec2& size);

  /// Sets up the OpenGL buffers.
  void CreateBuffers();

  /// Sets up the glyph shader.
  void CreateShader();

  /// Destroys the OpenGL buffers.
  void CleanUpBuffers();

  /// Destroys the glyph shader.
  void CleanUpShader();

  /// The ID of the glyph shader program.
  extern GLuint program_id;
  /// The ID of the VAO used to contain the VBO.
  extern GLuint vao_id;
  /// The ID of the VBO used to store the buffer-related data.
  extern GLuint vbo_id;
  /// The position of the view.
  extern glm::vec2 view_position;
  /// The size of the view, in rows/columns.
  extern glm::ivec2 view_size;
  /// The buffer of view data to render to the window.
  extern BufferList data;
}

#endif // ! VIEW_H
