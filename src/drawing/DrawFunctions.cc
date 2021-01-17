#include "DrawFunctions.h"

namespace term_engine::drawing {
  void WriteText(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& bounds, const std::string& text, const int& text_offset)
  {
    glyphs::GlyphList& data = glyph_set.GetData();
    glm::ivec2 pos = start_pos;
    int glyph_index = (pos.y * glyph_set.GetSize().y) + pos.y;

    for (int text_index = text_offset; text_index < text.length(); ++text_index) {
      if (pos.x > bounds.x) {
        pos.x = start_pos.x;
        ++pos.y;
      }

      if (pos.y > bounds.y) {
        break;
      }

      glyphs::GlyphData& glyph = data.at(glyph_index);
      glyph.character_ = text.at(text_index);
      glyph.foreground_color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
      glyph.background_color_ = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

      ++pos.x;
      ++glyph_index;
    }

    glyph_set.Dirty();
  }

  void DrawLine(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph)
  {

  }

  void DrawBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph)
  {

  }

  void DrawOutlinedBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph)
  {

  }

  void MoveGlyph(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos)
  {

  }

  void MoveLine(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos)
  {

  }

  void MoveBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& source_pos, const glm::ivec2& dest_pos)
  {

  }
}
