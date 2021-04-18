#include "BoxObject.h"

namespace term_engine::objects {
  BoxObject::BoxObject(const glm::ivec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params) :
    size_(size),
    params_(params)
  {
    position_ = position;
  }

  void BoxObject::Render(glyphs::BufferList& data)
  {
    const size_t index = ((size_t)position_.y * (size_t)glyphs::dimensions.x) + (size_t)position_.x;

    size_t x_count = 0;
    size_t y_count = 0;

    while (y_count < size_.y) {
      size_t set_index = index + (y_count * glyphs::dimensions.x) + x_count;
      data.at(set_index).Set(params_);

      ++x_count;

      if (x_count == size_.x) {
        x_count = 0;
        ++y_count;
      }
    }
  }
}
