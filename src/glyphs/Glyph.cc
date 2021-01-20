#include "Glyph.h"

namespace term_engine::glyphs {
  BufferData::BufferData() :
    texture_layer_(0.0f),
    offset_(glm::vec2(0.0f)),
    scale_(glm::vec2(1.0f)),
    foreground_color_(DEFAULT_FOREGROUND_COLOR),
    background_color_(DEFAULT_BACKGROUND_COLOR)
  {}

  BufferData::BufferData(const GLfloat& texture_layer, const glm::vec2& offset, const glm::vec2& scale, const glm::vec4& foreground_color, const glm::vec4& background_color) :
    texture_layer_(texture_layer),
    offset_(offset),
    scale_(scale),
    foreground_color_(foreground_color),
    background_color_(background_color)
  {}

  GlyphParams::GlyphParams(const char& character, const glm::vec2& offset, const glm::vec2& scale, const glm::vec4& foreground_color, const glm::vec4& background_color) :
    character_(character),
    offset_(offset),
    scale_(scale),
    foreground_color_(foreground_color),
    background_color_(background_color)
  {}

  GlyphData::GlyphData(const glm::uvec2& index) :
    index_(index),
    character_(' '),
    offset_(glm::vec2(0.0f)),
    scale_(glm::vec2(1.0f)),
    foreground_color_(DEFAULT_FOREGROUND_COLOR),
    background_color_(DEFAULT_BACKGROUND_COLOR)
  {}

  GlyphData::GlyphData(const glm::uvec2& index, const GlyphParams& data) :
    index_(index),
    character_(data.character_),
    offset_(data.offset_),
    scale_(data.scale_),
    foreground_color_(data.foreground_color_),
    background_color_(data.background_color_)
  {}

  void GlyphData::SetParams(const GlyphParams& glyph)
  {
    character_ = glyph.character_;
    offset_ = glyph.offset_;
    scale_ = glyph.scale_;
    foreground_color_ = glyph.foreground_color_;
    background_color_ = glyph.background_color_;
  }

  void GlyphData::Clear()
  {
    character_ = ' ';
    offset_ = glm::vec2(0.0f);
    foreground_color_ = DEFAULT_FOREGROUND_COLOR;
    background_color_ = DEFAULT_BACKGROUND_COLOR;
  }
}
