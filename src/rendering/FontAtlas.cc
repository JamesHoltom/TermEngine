#include <algorithm>
#include <array>
#include <freetype/ftbitmap.h>
#include <freetype/ftsizes.h>
#include "FontAtlas.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  FontAtlas::FontAtlas() :
    FontAtlas(std::filesystem::path(DEFAULT_FONT))
  {}

  FontAtlas::FontAtlas(const std::filesystem::path& filepath) :
    filepath_(filepath),
    whitespace_bbox_(glm::ivec2(), glm::ivec2(32), 0, 0),
    character_count_(0),
    is_loaded_(false),
    texture_dirty_(false),
    size_list_(),
    packer_(glm::ivec2(TEXTURE_SIZE))
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

    const uint64_t textureSize = 32 * 32 * sizeof(uint8_t);
    std::array<uint8_t, textureSize> whiteTexture;
    std::fill(whiteTexture.begin(), whiteTexture.end(), 255);

    packer_.Insert(whiteTexture.data(), glm::ivec2(32));

    SetFontSize(DEFAULT_FONT_SIZE);

    if (all_ok)
    {
      utility::logger->debug("Loaded font \"{}\".", filepath_.string());

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);

      is_loaded_ = true;
    }
    else
    {
      utility::logger->error("Failed to set up font \"{}\".", filepath_.string());

      DeleteTexture(texture_);

      return;
    }
  }

  FontAtlas::~FontAtlas()
  {
    if (is_loaded_)
    {
      DeleteTexture(texture_);

      size_list_.clear();

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

  std::string FontAtlas::GetFilePath() const
  {
    return filepath_.string();
  }

  glm::ivec2 FontAtlas::GetCharacterSize(uint32_t size)
  {
    SetFontSize(size);

    return glm::ivec2(face_->size->metrics.max_advance >> 6, ((face_->size->metrics.ascender - face_->size->metrics.descender) >> 6));
  }

  CharacterBB FontAtlas::GetCharacter(uint64_t character, uint32_t size)
  {
    if (!is_loaded_)
    {
      utility::logger->warn("Cannot get \"{}\" character from unloaded font \"{}\".", (char)(character), filepath_.string());

      return EMPTY_CHARACTER;
    }

    SetFontSize(size);

    CharacterList::iterator found_char = atlas_.find(character);

    if (character == '\0' || character == '\n' || character == '\r' || character == '\t')
    {
      return EMPTY_CHARACTER;
    }
    else if (found_char == atlas_.end())
    {
      return CreateCharTexture(character, size);
    }
    else
    {
      return found_char->second;
    }
  }

  CharacterBB FontAtlas::GetWhitespace() const
  {
    if (!is_loaded_)
    {
      utility::logger->warn("Cannot get whitespace from unloaded font \"{}\".", filepath_.string());

      return EMPTY_CHARACTER;
    }

    return whitespace_bbox_;
  }

  bool FontAtlas::IsLoaded() const
  {
    return is_loaded_;
  }

  void FontAtlas::Use() const
  {
    if (is_loaded_) {
      if (active_font_ != texture_.texture_id_)
      {
        glActiveTexture(GL_TEXTURE0 + texture_.texture_index_);
        glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);

        active_font_ = texture_.texture_id_;
      }
    }
    else
    {
      utility::logger->warn("Cannot use unloaded font \"{}\"!", filepath_.string());
    }
  }

  void FontAtlas::UpdateTexture()
  {
    if (texture_dirty_)
    {
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_.size_.x, texture_.size_.y, GL_RED, GL_UNSIGNED_BYTE, packer_.GetBufferData());

      texture_dirty_ = false;
    }
  }

  FontAtlasPtr FontAtlas::GetFontAtlas(const std::filesystem::path& filepath)
  {
    std::string key_name = filepath.string();

    atlas_cache_.emplace(key_name, std::make_shared<FontAtlas>(filepath));

    return atlas_cache_.at(key_name);
  }

  void FontAtlas::CleanUpFontAtlas()
  {
    for (std::pair<std::string, FontAtlasPtr> pair : atlas_cache_)
    {
      pair.second.reset();
    }

    atlas_cache_.clear();
  }

  CharacterBB FontAtlas::CreateCharTexture(uint64_t character, uint32_t size)
  {
    Use();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (utility::FTLog(FT_Load_Char(face_, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      // Character metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      glm::ivec2 character_size = glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows);
      FT_Pos character_baseline = (face_->size->metrics.ascender - face_->glyph->metrics.horiBearingY) >> 6;
      glm::ivec2 character_pos = packer_.Insert(face_->glyph->bitmap.buffer, character_size);
      CharacterBB bbox(character_pos, character_size, size, character_baseline);

      utility::logger->debug("Created character, \'{}\' with dimensions {},{} at pos {},{} and added to cache.", (char)character, character_size.x, character_size.y, character_pos.x, character_pos.y);

      auto new_character = atlas_.insert_or_assign(character, bbox);

      texture_dirty_ = true;
      character_count_++;

      return bbox;
    }
    else
    {
      utility::logger->error("Failed to load character \'{}\'.", character);

      return EMPTY_CHARACTER;
    }
  }

  void FontAtlas::SetFontSize(uint32_t size)
  {
    FontSizeList::const_iterator findSize = size_list_.find(size);

    if (findSize != size_list_.end())
    {
      if (utility::FTLog(FT_Activate_Size(findSize->second)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to activate existing font size for font \"{}\".", filepath_.string());
      }
    }
    else
    {
      FT_Size new_size;

      if (utility::FTLog(FT_New_Size(face_, &new_size)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to create font size for font \"{}\".", filepath_.string());
      }

      if (utility::FTLog(FT_Activate_Size(new_size)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to activate new font size for font \"{}\".", filepath_.string());
      }

      size_list_.insert(FontSizeList::value_type(size, new_size));
    }

    if (utility::FTLog(FT_Set_Pixel_Sizes(face_, 0, size)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to set font size for font \"{}\".", filepath_.string());
    }
  }

  uint32_t FontAtlas::active_font_ = 0;
  FontAtlasList FontAtlas::atlas_cache_;
}
