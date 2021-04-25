#include "OutlineBoxObject.h"

namespace term_engine::objects {
  OutlinedBoxObject::OutlinedBoxObject(const glm::vec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params) :
    size_(size),
    params_(params)
  {
    position_ = position;
  }

  // TODO: Amend size to act like a size, rather than an end position.
  void OutlinedBoxObject::Render(glyphs::BufferList& data)
  {
    size_t index = ((size_t)position_.y * glyphs::dimensions.x) + (size_t)position_.x;

    // Render the top and bottom sections of the box.
    for (size_t row = 0; row <= size_.x; ++row) {
      data.at(index + row).Set(params_);
      data.at(index + row + (((size_t)size_.y) * glyphs::dimensions.x)).Set(params_);
    }

    index += glyphs::dimensions.x;

    // Render the left and right sections of the box.
    for (size_t column = 0; column < (size_t)size_.y - 1; ++column) {
      data.at(index + (column * (size_t)glyphs::dimensions.x)).Set(params_);
      data.at(index + (column * (size_t)glyphs::dimensions.x) + size_.x).Set(params_);
    }
  }

  glyphs::GlyphParams OutlinedBoxObject::GetParams() const
  {
    return params_;
  }

  char OutlinedBoxObject::GetCharacter() const
  {
    return params_.character_;
  }

  glm::vec4 OutlinedBoxObject::GetForegroundColor() const
  {
    return params_.foreground_color_;
  }

  glm::vec4 OutlinedBoxObject::GetBackgroundColor() const
  {
    return params_.background_color_;
  }

  void OutlinedBoxObject::SetParams(const glyphs::GlyphParams& params)
  {
    params_ = params;
    is_dirty = true;
  }

  void OutlinedBoxObject::SetCharacter(const char& character)
  {
    params_.character_ = character;
    is_dirty = true;
  }

  void OutlinedBoxObject::SetForegroundColor(const glm::vec4& fg_color)
  {
    params_.foreground_color_ = fg_color;
    is_dirty = true;
  }

  void OutlinedBoxObject::SetBackgroundColor(const glm::vec4& bg_color)
  {
    params_.background_color_ = bg_color;
    is_dirty = true;
  }
}
