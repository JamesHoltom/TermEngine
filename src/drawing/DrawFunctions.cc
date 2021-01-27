#include "DrawFunctions.h"
#include "../logging/Logger.h"

namespace term_engine::drawing {
  void WriteText(
    glyphs::GlyphSet& glyph_set,
    const glm::ivec2& start_pos,
    const glm::ivec2& end_pos,
    const std::string& text,
    const glm::vec4& foreground_color,
    const glm::vec4& background_color,
    const int& text_offset)
  {
    glyphs::GlyphList& data = glyph_set.GetData();
    glm::ivec2 pos = start_pos;
    int glyph_index = (pos.y * glyph_set.GetSize().x) + pos.x;

    for (int text_index = text_offset; text_index < text.length(); ++text_index) {
      if (pos.x > end_pos.x) {
        pos.x = start_pos.x;
        ++pos.y;
        glyph_index = (pos.y * glyph_set.GetSize().x) + pos.x;
      }

      if (text.at(text_index) == '\n') {
        pos.x = start_pos.x;
        ++pos.y;
        glyph_index = (pos.y * glyph_set.GetSize().x) + pos.x;

        continue;
      }

      if (pos.y > end_pos.y) {
        break;
      }

      glyphs::GlyphData& glyph = data.at(glyph_index);
      glyph.foreground_color_ = foreground_color;
      glyph.background_color_ = background_color;
      glyph.character_ = text.at(text_index);

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
    glyphs::GlyphList& data = glyph_set.GetData();
    const size_t set_width = glyph_set.GetSize().x;
    const glm::ivec2 dimensions = end_pos - start_pos;

    size_t index = (start_pos.y *  set_width) + start_pos.x;

    // Render the top and bottom sections of the box.
    for (size_t row = index; row < dimensions.x; ++row) {
      data.at(row).SetParams(glyph);
      data.at(row + (((size_t)dimensions.y - 1) * set_width)).SetParams(glyph);
    }

    index += set_width;

    // Render the left and right sections of the box.
    for (int column = 0; column < dimensions.y - 2; ++column) {
      data.at(index + (column * set_width) + start_pos.x).SetParams(glyph);
      data.at(index + (column * set_width) + end_pos.x - 1).SetParams(glyph);
    }
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
