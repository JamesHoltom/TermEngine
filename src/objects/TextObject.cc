#include "TextObject.h"

namespace term_engine::objects {
  TextObject::TextObject(const std::string& text, const glm::ivec2& position, const glm::vec4& fg_color, const glm::vec4& bg_color, const glm::ivec2& size) :
    text_(text),
    params_(' ', fg_color, bg_color)
  {
    position_ = position;

    if (size != glm::ivec2(-1)) {
      size_ = size;
    }
    else {
      size_ = glyphs::dimensions - glm::uvec2(1);
    }
  }

  void TextObject::Render(glyphs::BufferList& data)
  {
    glm::ivec2 pos = position_;
    int glyph_index = (pos.y * glyphs::dimensions.x) + pos.x;
    char character;

    for (int text_index = 0; text_index < text_.length(); ++text_index) {
      character = text_.at(text_index);

      if (pos.x > size_.x) {
        pos.x = position_.x;
        ++pos.y;
        glyph_index = (pos.y * glyphs::dimensions.x) + pos.x;
      }

      if (character == '\n') {
        pos.x = position_.x;
        ++pos.y;
        glyph_index = (pos.y * glyphs::dimensions.x) + pos.x;

        continue;
      }

      if (pos.y > size_.y) {
        break;
      }

      params_.character_ = character;
      data.at(glyph_index).Set(params_);

      ++pos.x;
      ++glyph_index;
    }

    params_.character_ = ' ';
  }

  std::string TextObject::GetText() const
  {
    return text_;
  }

  void TextObject::SetText(const std::string& text)
  {
    text_ = text;
    is_dirty = true;
  }
}