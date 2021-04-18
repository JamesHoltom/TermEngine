/// @author James Holtom

#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include <string>
#include <glm/glm.hpp>

#include "BaseObject.h"

namespace term_engine::objects {
  class TextObject : public BaseObject {
  public:
    /// Creates the textbox with the given text and parameters.
    /**
     * @param[in] text
     */
    TextObject(const std::string& text, const glm::ivec2& position, const glm::vec4& fg_color, const glm::vec4& bg_color, const glm::ivec2& size = glm::ivec2(-1));

    /// Renders the textbox to the given glyph set.
    /**
      * @param[in] data
      */
    void Render(glyphs::BufferList& data);

    std::string GetText() const;

    void SetText(const std::string& text);

  private:
    std::string text_;
    glm::ivec2 size_;

    /// The character and colours to use when rendering the box.
    glyphs::GlyphParams params_;
  };
}

#endif // ! TEXT_OBJECT_H
