/// @author James Holtom

#ifndef OUTLINED_BOX_OBJECT_H
#define OUTLINED_BOX_OBJECT_H

#include "BaseObject.h"

namespace term_engine::objects {
  class OutlinedBoxObject : public BaseObject {
  public:
    /// Creates the box with the given size and parameters.
    /**
     * @param[in] size   The size of the box to create.
     * @param[in] params The character and colours to use when rendering the box.
     */
    OutlinedBoxObject(const glm::vec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params);

    /// Renders the box to the given glyph set.
    /**
     * @param[in] glyph_set A pointer to the glyph set to render the box to.
     */
    void Render(glyphs::BufferList& data);

    glyphs::GlyphParams GetParams() const;
    char GetCharacter() const;
    glm::vec4 GetForegroundColor() const;
    glm::vec4 GetBackgroundColor() const;

    void SetParams(const glyphs::GlyphParams& params);
    void SetCharacter(const char& character);
    void SetForegroundColor(const glm::vec4& fg_color);
    void SetBackgroundColor(const glm::vec4& bg_color);

  protected:
    /// The size of the box.
    glm::ivec2 size_;

    /// The character and colours to use when rendering the box.
    glyphs::GlyphParams params_;
  };
}

#endif // ! OUTLINED_BOX_OBJECT_H
