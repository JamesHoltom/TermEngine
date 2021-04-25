#include "BoxObject.h"

namespace term_engine::objects {
  BoxObject::BoxObject(const glm::vec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params) :
    size_(size),
    params_(params)
  {
    position_ = position;
  }

  // @todo Add bounds checking for objects that're partially of fully obscured from view.
  void BoxObject::Render(glyphs::BufferList& data)
  {
    const size_t index = ((size_t)position_.y * (size_t)glyphs::dimensions.x) + (size_t)position_.x;

    size_t x_count = 0;
    size_t y_count = 0;

    while (y_count < size_.y) {
      size_t set_index = index + (y_count * glyphs::dimensions.x) + x_count;

      if (set_index / glyphs::dimensions.x) {
        break;
      }
      else {
        data.at(set_index).Set(params_);
      }

      ++x_count;

      if (x_count % size_.x == 0 || index % glyphs::dimensions.x == 0) {
        ++y_count;
      }
    }
  }

  glyphs::GlyphParams BoxObject::GetParams() const
  {
    return params_;
  }

  char BoxObject::GetCharacter() const
  {
    return params_.character_;
  }

  glm::vec4 BoxObject::GetForegroundColor() const
  {
    return params_.foreground_color_;
  }

  glm::vec4 BoxObject::GetBackgroundColor() const
  {
    return params_.background_color_;
  }

  void BoxObject::SetParams(const glyphs::GlyphParams& params)
  {
    params_ = params;
    is_dirty = true;
  }

  void BoxObject::SetCharacter(const char& character)
  {
    params_.character_ = character;
    is_dirty = true;
  }

  void BoxObject::SetForegroundColor(const glm::vec4& fg_color)
  {
    params_.foreground_color_ = fg_color;
    is_dirty = true;
  }

  void BoxObject::SetBackgroundColor(const glm::vec4& bg_color)
  {
    params_.background_color_ = bg_color;
    is_dirty = true;
  }
}
