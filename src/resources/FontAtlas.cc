#include <array>
#include <filesystem>

#include "FontAtlas.h"
#include "../data/Uniform.h"
#include "../logging/Logger.h"
#include "../system/FileFunctions.h"

namespace term_engine::fonts {
  std::string font_path;
  FT_Int font_size;
  FT_Face font_face;
  GlyphList font_atlas;
  GLuint texture_id;
  GLuint glyph_count;
  glm::uvec2 next_pos_;
  GLuint max_height_;

  bool Init()
  {
    return SetFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
  }

  void CleanUp()
  {
    _RemoveFont();
  }

  GlyphBB GetCharacter(const FT_ULong& character)
  {
    auto found_char = font_atlas.find(character);

    if (character == '\0')
    {
      return EMPTY_GLYPH;
    }
    else if (found_char == font_atlas.end()) {
      return _LoadChar(character);
    }
    else {
      return found_char->second;
    }
  }

  std::pair<bool, GlyphBB> _CreateCharTexture(const FT_ULong& character)
  {
    if (FT::Log(FT_Load_Char(font_face, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      // Glyph metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint glyph_width = font_face->glyph->bitmap.width;
      GLuint glyph_height = font_face->glyph->bitmap.rows;
      GLint glyph_baseline = (font_face->size->metrics.ascender - font_face->glyph->metrics.horiBearingY) >> 6;

      // If the next glyph will spill out of the bottom-right corner of the texture, stop and return an error.
      if (next_pos_.y + glyph_height > TEXTURE_SIZE && next_pos_.x + glyph_width > TEXTURE_SIZE)
      {
        logging::logger->warn("The texture for this font is full!");

        return std::make_pair(false, EMPTY_GLYPH);
      }

      glBindTexture(GL_TEXTURE_2D, texture_id);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, glyph_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, glyph_height);
      glTexSubImage2D(GL_TEXTURE_2D, 0, next_pos_.x, next_pos_.y, glyph_width, glyph_height, GL_RED, GL_UNSIGNED_BYTE, font_face->glyph->bitmap.buffer);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      GlyphBB bbox = { next_pos_, glm::ivec2(glyph_width, glyph_height), glyph_baseline };

      logging::logger->debug("Created character, \'{}\' with dimensions {},{} at pos {},{} and added to cache.", (char)character, glyph_width, glyph_height, next_pos_.x, next_pos_.y);

      if (glyph_height > max_height_)
      {
        max_height_ = glyph_height;
      }

      if (next_pos_.x + glyph_width > TEXTURE_SIZE)
      {
        next_pos_.x = 0;
        next_pos_.y += max_height_;
        max_height_ = 0;
      }
      else
      {
        next_pos_.x += glyph_width;
      }

      return std::make_pair(true, bbox);
    }
    else
    {
      logging::logger->error("Failed to load character \'{}\'.", character);

      return std::make_pair(false, EMPTY_GLYPH);
    }
  }

  GlyphBB _LoadChar(const FT_ULong& character)
  {
    std::pair<bool, GlyphBB> result = _CreateCharTexture(character);

    if (result.first) {
      auto new_glyph = font_atlas.emplace(std::make_pair(character, result.second));

      glyph_count++;

      return result.second;
    }
    else {
      return EMPTY_GLYPH;
    }
  }

  bool SetFont(const std::string& filename, const FT_Int& size)
  {
    const std::filesystem::path fullFontPath = system::SearchForFontPath(filename);

    if (!std::filesystem::exists(fullFontPath)) {
      logging::logger->warn("Attempting to set font to one that doesn't exist: {}", fullFontPath);

      return false;
    }

    if (fullFontPath != font_path) {
      if (font_path != "") {
        _RemoveFont();
      }

      if (FT::Log(FT_New_Face(FT::font_library, fullFontPath.c_str(), 0, &font_face)) != FT_Err_Ok) {
        logging::logger->error("Failed to create font face.");

        return false;
      }
    }

    if (FT::Log(FT_Set_Pixel_Sizes(font_face, 0, size)) != FT_Err_Ok) {
      logging::logger->error("Failed to set font size.");

      return false;
    }

    font_path = fullFontPath;
    font_size = size;

    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, TEXTURE_SIZE, TEXTURE_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (auto& glyph : font_atlas) {
      _CreateCharTexture(glyph.first);
    }

    data::SetFontSize(glm::vec2(size));

    return true;
  }

  void _RemoveFont()
  {
    glDeleteTextures(1, &texture_id);
    
    if (FT::Log(FT_Done_Face(font_face)) != FT_Err_Ok)
    {
      logging::logger->error("Failed to remove font face.");
    }
  }

  std::string GetFontPath()
  {
    return font_path;
  }

  std::string GetDefaultFontPath()
  {
    return std::string(DEFAULT_FONT);
  }

  int GetFontSize()
  {
    return font_size;
  }

  int GetDefaultFontSize()
  {
    return DEFAULT_FONT_SIZE;
  }
}
