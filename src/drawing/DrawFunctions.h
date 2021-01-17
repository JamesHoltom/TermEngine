/// @author James Holtom

#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <string>
#include <glm/glm.hpp>

#include "../glyphs/GlyphSet.h"

namespace term_engine::drawing {
  void WriteText(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& bounds, const std::string& text, const int& text_offset = 0);
  void DrawLine(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);
  void DrawBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);
  void DrawOutlinedBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);

  void MoveGlyph(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos);
  void MoveLine(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos);
  void MoveBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos);
}

#endif
