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
    glyphs::GlyphList& data = glyph_set.GetData();
    const glm::uvec2 set_size = glyph_set.GetSize();
    const glm::uvec2 dimensions = end_pos - start_pos;
    const size_t index = ((size_t)start_pos.y * (size_t)set_size.x) + (size_t)start_pos.x;

    size_t x_count = 0;
    size_t y_count = 0;

    while (y_count <= dimensions.y) {
      data.at(index + (y_count * set_size.x) + x_count).SetParams(glyph);

      if (x_count == dimensions.x) {
        x_count = 0;
        ++y_count;
      }
      else {
        ++x_count;
      }
    }
  }

  void DrawOutlinedBox(glyphs::GlyphSet& glyph_set, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glyphs::GlyphParams& glyph)
  {
    glyphs::GlyphList& data = glyph_set.GetData();
    const size_t set_width = glyph_set.GetSize().x;
    const glm::ivec2 dimensions = end_pos - start_pos + glm::ivec2(1);

    size_t index = (start_pos.y *  set_width) + start_pos.x;

    // Render the top and bottom sections of the box.
    for (size_t row = 0; row < dimensions.x; ++row) {
      data.at(index + row).SetParams(glyph);
      data.at(index + row + (((size_t)dimensions.y - 1) * set_width)).SetParams(glyph);
    }

    index += set_width;

    // Render the left and right sections of the box.
    for (int column = 0; column < dimensions.y - 2; ++column) {
      data.at(index + (column * set_width)).SetParams(glyph);
      data.at(index + (column * set_width) + dimensions.x - 1).SetParams(glyph);
    }
  }
}
