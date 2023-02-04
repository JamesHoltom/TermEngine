#include <array>
#include <freetype/ftbitmap.h>
#include "FontAtlas.h"
#include "ShaderProgram.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  FontAtlas::FontAtlas(const std::filesystem::path& filepath, const FT_Int& size) :
    filepath_(filepath),
    size_(size),
    is_loaded_(false),
    next_pos_(glm::uvec2(0))
  {
    bool all_ok = true;

    texture_ = AllocateTexture(glm::ivec2(TEXTURE_SIZE), GL_R8, 0);

    if (texture_.texture_id_ == -1)
    {
      utility::logger->error("Failed to allocate texture for font \"{}\".", filepath_.string());

      return;
    }

    if (utility::FTLog(FT_New_Face(utility::font_library, filepath.c_str(), 0, &face_)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to load font \"{}\".", filepath_.string());

      all_ok = false;
    }

    if (utility::FTLog(FT_Set_Pixel_Sizes(face_, 0, size)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to set font size for font \"{}\".", filepath_.string());

      all_ok = false;
    }

    if (all_ok)
    {
      utility::logger->debug("Loaded font \"{}\" with size {}.", filepath_.string(), size);

      ResetCharacterSize();

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);

      is_loaded_ = true;
    }
    else
    {
      DeleteTexture(texture_);

      return;
    }
  }

  FontAtlas::~FontAtlas()
  {
    if (is_loaded_)
    {
      DeleteTexture(texture_);

      if (utility::FTLog(FT_Done_Face(face_)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to remove font \"{}\".", filepath_.string());
      }
      else
      {
        utility::logger->debug("Removed font \"{}\".", filepath_.string());
      }
    }
  }

  std::string FontAtlas::GetFilepath() const
  {
    return filepath_;
  }

  FT_Int FontAtlas::GetFontSize() const
  {
    return size_;
  }

  glm::ivec2 FontAtlas::GetCharacterSize() const
  {
    return character_size_;
  }

  CharacterBB FontAtlas::GetCharacter(const FT_ULong& character)
  {
    CharacterList::iterator found_char = atlas_.find(character);

    if (character == '\0' || character == '\n' || character == '\r' || character == '\t')
    {
      return EMPTY_CHARACTER;
    }
    else if (found_char == atlas_.end())
    {
      return CreateCharTexture(character);
    }
    else
    {
      return found_char->second;
    }
  }

  void FontAtlas::SetCharacterSize(const glm::ivec2& character_size)
  {
    character_size_ = character_size;
  }

  void FontAtlas::ResetCharacterSize()
  {
    character_size_ = glm::ivec2(face_->size->metrics.max_advance >> 6, ((face_->size->metrics.ascender - face_->size->metrics.descender) >> 6));
  }

  void FontAtlas::Use() const
  {
    glActiveTexture(GL_TEXTURE0 + texture_.texture_index_);
    glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);
  }

  CharacterBB FontAtlas::CreateCharTexture(const FT_ULong& character)
  {
    glActiveTexture(GL_TEXTURE0 + texture_.texture_index_);
    glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (utility::FTLog(FT_Load_Char(face_, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      // Character metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint character_width = face_->glyph->bitmap.width;
      GLuint character_height = face_->glyph->bitmap.rows;
      GLint character_baseline = (face_->size->metrics.ascender - face_->glyph->metrics.horiBearingY) >> 6;

      // If the next character will spill out of the bottom-right corner of the texture, stop and return an error.
      if (next_pos_.y + character_height + 1 > TEXTURE_SIZE && next_pos_.x + character_width + 1 > TEXTURE_SIZE)
      {
        utility::logger->warn("The texture for this font is full!");

        return EMPTY_CHARACTER;
      }

      glPixelStorei(GL_UNPACK_ROW_LENGTH, character_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, character_height);
      glTexSubImage2D(GL_TEXTURE_2D, 0, next_pos_.x, next_pos_.y, character_width, character_height, GL_RED, GL_UNSIGNED_BYTE, face_->glyph->bitmap.buffer);

      CharacterBB bbox = { next_pos_, glm::ivec2(character_width, character_height), character_baseline };

      utility::logger->debug("Created character, \'{}\' with dimensions {},{} at pos {},{} and added to cache.", (char)character, character_width, character_height, next_pos_.x, next_pos_.y);

      if (character_height > max_height_)
      {
        max_height_ = character_height;
      }

      if (next_pos_.x + character_width > TEXTURE_SIZE)
      {
        next_pos_.x = 0;
        next_pos_.y += max_height_ + 1;
        max_height_ = 0;
      }
      else
      {
        next_pos_.x += character_width + 1;
      }
      
      auto new_character = atlas_.insert_or_assign(character, bbox);

      character_count_++;

      return bbox;
    }
    else
    {
      utility::logger->error("Failed to load character \'{}\'.", character);

      return EMPTY_CHARACTER;
    }
  }

  void FontAtlas::ClearCache()
  {
    rendering::ClearTexture(texture_, glm::vec4());

    next_pos_ = glm::uvec2();
  }
}
