#include <array>
#include <filesystem>
#include "FontAtlas.h"
#include "../data/Uniform.h"
#include "../logging/Logger.h"
#include "../objects/Object.h"
#include "../system/FileFunctions.h"

namespace term_engine::fonts {
  std::string font_path;
  int font_size;
  glm::ivec2 character_size;
  FT_Face font_face;
  CharacterList font_atlas;
  GLuint texture_id;
  GLuint character_count;
  glm::uvec2 next_pos;
  GLuint max_height;
  unsigned int tab_size = DEFAULT_TAB_SIZE;

  bool Init()
  {
    glGenTextures(1, &texture_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, TEXTURE_SIZE, TEXTURE_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return SetFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
  }

  void CleanUp()
  {
    _RemoveFont();

    glDeleteTextures(1, &texture_id);
  }

  CharacterBB GetCharacter(const FT_ULong& character)
  {
    auto found_char = font_atlas.find(character);

    if (character == '\0' || character == '\n' || character == '\r' || character == '\t')
    {
      return EMPTY_CHARACTER;
    }
    else if (found_char == font_atlas.end())
    {
      return _LoadChar(character);
    }
    else
    {
      return found_char->second;
    }
  }

  std::pair<bool, CharacterBB> _CreateCharTexture(const FT_ULong& character)
  {
    if (FT::Log(FT_Load_Char(font_face, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      // Character metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint character_width = font_face->glyph->bitmap.width;
      GLuint character_height = font_face->glyph->bitmap.rows;
      GLint character_baseline = (font_face->size->metrics.ascender - font_face->glyph->metrics.horiBearingY) >> 6;

      // If the next character will spill out of the bottom-right corner of the texture, stop and return an error.
      if (next_pos.y + character_height > TEXTURE_SIZE && next_pos.x + character_width > TEXTURE_SIZE)
      {
        logging::logger->warn("The texture for this font is full!");

        return std::make_pair(false, EMPTY_CHARACTER);
      }

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, character_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, character_height);
      glTexSubImage2D(GL_TEXTURE_2D, 0, next_pos.x, next_pos.y, character_width, character_height, GL_RED, GL_UNSIGNED_BYTE, font_face->glyph->bitmap.buffer);

      CharacterBB bbox = { next_pos, glm::ivec2(character_width, character_height), character_baseline };

      logging::logger->debug("Created character, \'{}\' with dimensions {},{} at pos {},{} and added to cache.", (char)character, character_width, character_height, next_pos.x, next_pos.y);

      if (character_height > max_height)
      {
        max_height = character_height;
      }

      if (next_pos.x + character_width > TEXTURE_SIZE)
      {
        next_pos.x = 0;
        next_pos.y += max_height;
        max_height = 0;
      }
      else
      {
        next_pos.x += character_width;
      }
      
      auto new_character = font_atlas.insert_or_assign(character, bbox);

      character_count++;

      return std::make_pair(true, bbox);
    }
    else
    {
      logging::logger->error("Failed to load character \'{}\'.", character);

      return std::make_pair(false, EMPTY_CHARACTER);
    }
  }

  CharacterBB _LoadChar(const FT_ULong& character)
  {
    return _CreateCharTexture(character).second;
  }

  bool SetFont(const std::string& filename, const FT_Int& size)
  {
    const std::filesystem::path fullFontPath = system::SearchForResourcePath(filename);

    if (!std::filesystem::exists(fullFontPath))
    {
      logging::logger->warn("Attempting to set font to one that doesn't exist: {}", fullFontPath);

      return false;
    }

    if (fullFontPath != font_path) 
    {
      if (font_path != "")
      {
        _RemoveFont();
      }

      if (FT::Log(FT_New_Face(FT::font_library, fullFontPath.c_str(), 0, &font_face)) != FT_Err_Ok)
      {
        logging::logger->error("Failed to create font face.");

        return false;
      }
    }

    _ClearCache();

    if (FT::Log(FT_Set_Pixel_Sizes(font_face, 0, size)) != FT_Err_Ok)
    {
      logging::logger->error("Failed to set font size.");

      return false;
    }

    font_path = fullFontPath;
    font_size = size;

    logging::logger->debug("Set font to {} with size {}.", font_path, font_size);

    ResetCharacterSize();

    for (auto& character : font_atlas)
    {
      _CreateCharTexture(character.first);
    }

    return true;
  }

  void _RemoveFont()
  {
    if (FT::Log(FT_Done_Face(font_face)) != FT_Err_Ok)
    {
      logging::logger->error("Failed to remove font face.");
    }
  }

  void _ClearCache()
  {
    GLubyte clearColour[4] = { 0, 0, 0, 0 };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glClearTexImage(texture_id, 0, GL_RED, GL_UNSIGNED_BYTE, &clearColour);

    next_pos = glm::uvec2(0);
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

  glm::ivec2 GetCharacterSize()
  {
    return character_size;
  }

  void SetCharacterSize(const glm::ivec2& characterSize)
  {
    character_size = characterSize;
  }

  void ResetCharacterSize()
  {
    character_size = glm::ivec2(font_face->size->metrics.max_advance >> 6, ((font_face->size->metrics.ascender - font_face->size->metrics.descender) >> 6));

    objects::Object::Dirty();
  }
}
