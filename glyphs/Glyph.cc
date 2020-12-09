#include "Glyph.h"

namespace term_engine::glyphs {
  GlyphData::GlyphData(const glm::uvec2& index) :
    index_(index),
    offset_(glm::vec2(0.0f)),
    character_(' '),
    foreground_color_(DEFAULT_FOREGROUND_COLOR),
    background_color_(DEFAULT_BACKGROUND_COLOR)
  {}

  void GlyphData::Clear() {
    character_ = ' ';
    offset_ = glm::vec2(0.0f);
    foreground_color_ = DEFAULT_FOREGROUND_COLOR;
    background_color_ = DEFAULT_BACKGROUND_COLOR;
  }
}
