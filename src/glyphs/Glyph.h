/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_H
#define GLYPH_H

#include <ostream>
#include <vector>

#include "../utility/LoggingUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  constexpr glm::vec4 DEFAULT_FOREGROUND_COLOR = glm::vec4(1.0f);
  constexpr glm::vec4 DEFAULT_BACKGROUND_COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  struct BufferData {
    glm::vec2 vertex_offset_;
    GLfloat texture_offset_;
    glm::vec4 foreground_color_;
    glm::vec4 background_color_;

    BufferData() : vertex_offset_(glm::vec2(0.0f)), texture_offset_(-1.0f), foreground_color_(DEFAULT_FOREGROUND_COLOR), background_color_(DEFAULT_BACKGROUND_COLOR) {};

    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Vertex offset:     " << data.vertex_offset_ << std::endl
        << "Texture offset:    " << data.texture_offset_ << std::endl
        << "Foreground colour: " << data.foreground_color_ << std::endl
        << "Background colour: " << data.background_color_ << std::endl;
    }
  };

  struct GlyphParams {
    GlyphParams(const char& character, const glm::vec4& fg_color, const glm::vec4& bg_color);

    char character_;
    glm::vec4 foreground_color_;
    glm::vec4 background_color_;
  };

  struct GlyphData {
    GlyphData() = delete;
    GlyphData(const glm::uvec2& index);
    GlyphData(const glm::uvec2& index, const GlyphParams& glyph);

    void Clear();

    friend std::ostream& operator<<(std::ostream& os, const GlyphData& glyph) {
      return os << std::endl
        << "Position:          " << glyph.index_ << std::endl
        << "Offset:            " << glyph.offset_ << std::endl
        << "Character:         " << glyph.character_ << std::endl
        << "Foreground colour: " << glyph.foreground_color_ << std::endl
        << "Background colour: " << glyph.background_color_ << std::endl;
    }

    const glm::uvec2 index_;
    glm::vec2 offset_;
    char character_;
    glm::vec4 foreground_color_;
    glm::vec4 background_color_;

  };

  typedef std::vector<GlyphData> GlyphList;
  typedef std::vector<BufferData> BufferList;
}

#endif // ! GLYPH_H
