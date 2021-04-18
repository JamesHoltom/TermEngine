/// @author James Holtom

#ifndef UNIFORM_H
#define UNIFORM_H

#include "../fonts/FontAtlas.h"
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
    /// The texture size of the current font.
    glm::vec2 font_size_;
    /// The position of the glyph set in the window.
    glm::vec2 set_position_;
    /// The amount of padding around a glyph.
    glm::vec2 set_padding_;
    /// The amount of spacing between glyphs.
    glm::vec2 set_spacing_;
  };

  /// Initialises the Uniform Buffer Object.
  void Init();

  /// Deletes the Uniform Buffer Object.
  void CleanUp();

  /// Updates the projection matrix in the UBO to the window's current size.
  void SetProjection(const glm::ivec2& window_size);

  /// Returns the scale of a glyph on the screen.
  /**
   * @returns The scale a glyph.
   */
  glm::vec2 GetFontSize();

  /// Resizes the glyphs to the given scale.
  /**
   * @param[in] size The scale to resize the glyphs to.
   */
  void SetFontSize(const glm::vec2& size);

  /// Returns the position of the glyph set on the screen.
  /**
   * @returns The position of the glyph set.
   */
  glm::vec2 GetPosition();

  /// Returns the size of the padding around the glyphs.
  /**
   * @returns The size of the glyph padding.
   */
  glm::vec2 GetPadding();

  /// Returns the size of the spacing between glyphs.
  /**
   * @returns The size of the glyph spacing.
   */
  glm::vec2 GetSpacing();

  /// Sets the position of the glyph set on the screen.
  /**
   * @param[in] position The position to move the set to.
   */
  void SetPosition(const glm::vec2& position);

  /// Sets the size of the padding around the glyphs.
  /**
   * @param[in] padding The new padding size to use.
   */
  void SetPadding(const glm::vec2& padding);

  /// Sets the size of the spacing between glyphs.
  /**
   * @param[in] spacing The new spacing size to use.
   */
  void SetSpacing(const glm::vec2& spacing);

}

#endif // ! UNIFORM_H
