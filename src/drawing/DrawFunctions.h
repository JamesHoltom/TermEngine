/// @author James Holtom

#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <string>
#include <glm/glm.hpp>

#include "../glyphs/GlyphSet.h"

namespace term_engine::drawing {
  void WriteText(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const std::string& text, const glm::vec4& foreground_color, const glm::vec4& background_color, const int& text_offset = 0);
  void DrawLine(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);
  void DrawBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);
  void DrawOutlinedBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph);
}

#endif // ! DRAW_FUNCTIONS_H
