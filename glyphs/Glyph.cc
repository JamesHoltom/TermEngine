#include "Glyph.h"

namespace term_engine::glyphs {
  GlyphParams::GlyphParams(const char& character, const glm::vec4& fg_color, const glm::vec4& bg_color) :
    character_(character),
    foreground_color_(fg_color),
    background_color_(bg_color) {}

  GlyphData::GlyphData(const glm::uvec2& index) :
    index_(index),
    offset_(glm::vec2(0.0f)),
    character_(' '),
    foreground_color_(DEFAULT_FOREGROUND_COLOR),
    background_color_(DEFAULT_BACKGROUND_COLOR) {}

  GlyphData::GlyphData(const glm::uvec2& index, const GlyphParams& data) :
    index_(index),
    offset_(glm::vec2(0.0f)),
    character_(data.character_),
    foreground_color_(data.foreground_color_),
    background_color_(data.background_color_) {}

  void GlyphData::Clear() {
    character_ = ' ';
    offset_ = glm::vec2(0.0f);
    foreground_color_ = DEFAULT_FOREGROUND_COLOR;
    background_color_ = DEFAULT_BACKGROUND_COLOR;
  }
}
