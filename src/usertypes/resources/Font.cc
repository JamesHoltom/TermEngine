#include <wchar.h>
#include "Font.h"
#include "../../system/FileFunctions.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  Font::Font(const std::filesystem::path& filepath, FT_Face face) :
    BaseResource(filepath.string()),
    face_(face),
    atlas_({}),
    character_count_(0),
    size_list_(),
    packer_(glm::ivec2(TEXTURE_SIZE))
  {
    texture_ = rendering::TexturePtr(rendering::AllocateTexture(glm::ivec2(TEXTURE_SIZE), GL_R8, 0));

    const uint64_t textureSize = 32 * 32 * sizeof(uint8_t);
    std::array<uint8_t, textureSize> whiteTexture;
    std::fill(whiteTexture.begin(), whiteTexture.end(), 255);

    packer_.Insert(whiteTexture.data(), glm::ivec2(32));

    SetSize(DEFAULT_FONT_SIZE);

    utility::logger->debug("Loaded font resource with filepath \"{}\".", filepath.string());
  }

  Font::~Font()
  {
    size_list_.clear();

    if (utility::FTLog(FT_Done_Face(face_)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to remove font \"{}\".", name_);
    }
    else
    {
      utility::logger->debug("Removed font \"{}\".", name_);
    }

    texture_.reset();

    utility::logger->debug("Destroyed font resource with filepath \"{}\".", name_);
  }

  std::string Font::GetResourceType() const
  {
    return FONT_TYPE;
  }

  glm::ivec2 Font::GetCharacterSize(uint32_t size)
  {
    if (size == 0)
    {
      utility::logger->warn("Cannot get character size for a font size of 0!");

      return glm::ivec2();
    }

    FontSizeList::iterator findSize = size_list_.find(size);

    if (findSize == size_list_.end())
    {
      findSize = AddSize(size);
    }

    return glm::ivec2(findSize->second->metrics.max_advance >> 6, ((findSize->second->metrics.ascender - findSize->second->metrics.descender) >> 6));
  }

  CharacterBB Font::GetCharacter(char16_t character, uint32_t size)
  {
    if (size == 0)
    {
      utility::logger->warn("Cannot get character with a font size of 0!");

      return EMPTY_CHARACTER;
    }

    SetSize(size);

    CharacterList::iterator found_char = atlas_.find(std::pair<wchar_t, uint32_t>(character, size));

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

  void Font::UpdateTexture()
  {
    assert(texture_);

    if (texture_dirty_)
    {
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_->size_.x, texture_->size_.y, GL_RED, GL_UNSIGNED_BYTE, packer_.GetTextureData());

      texture_dirty_ = false;
    }
  }

  void Font::Use()
  {
    assert(texture_);

    glActiveTexture(GL_TEXTURE0 + texture_->texture_unit_);
    glBindTexture(GL_TEXTURE_2D, texture_->texture_id_);
  }

  CharacterBB Font::CreateCharTexture(uint64_t character, uint32_t size)
  {
    if (size == 0) {
      utility::logger->warn("Cannot use a font size of 0!");

      return EMPTY_CHARACTER;
    }

    Use();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    uint64_t glyph_index = FT_Get_Char_Index(face_, character);

    if (utility::FTLog(FT_Load_Char(face_, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      // Character metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      glm::ivec2 character_size = glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows);
      FT_Pos character_baseline = (face_->size->metrics.ascender - face_->glyph->metrics.horiBearingY) >> 6;
      glm::ivec2 character_pos = packer_.Insert(face_->glyph->bitmap.buffer, character_size);
      CharacterBB bbox(character_pos, character_size, size, character_baseline);

      utility::logger->debug("Created character {} ({}) with dimensions {},{} at pos {},{} and added to cache.", glyph_index, character, character_size.x, character_size.y, character_pos.x, character_pos.y);

      auto new_character = atlas_.insert_or_assign(std::pair<wchar_t, uint32_t>(character, size), bbox);

      character_count_++;
      texture_dirty_ = true;

      return bbox;
    }
    else
    {
      utility::logger->error("Failed to load character.");

      return EMPTY_CHARACTER;
    }
  }

  FontSizeList::iterator Font::AddSize(uint32_t size)
  {
    if (size == 0) {
      utility::logger->warn("Cannot use a font size of 0!");

      return size_list_.end();
    }

    FT_Size new_size;

    if (utility::FTLog(FT_New_Size(face_, &new_size)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to create font size for font \"{}\".", name_);
    }

    if (utility::FTLog(FT_Activate_Size(new_size)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to activate font size for font \"{}\".", name_);
    }

    if (utility::FTLog(FT_Set_Pixel_Sizes(face_, 0, size)) != FT_Err_Ok)
    {
      utility::logger->error("Failed to set font size for font \"{}\".", name_);
    }

    return size_list_.insert(FontSizeList::value_type(size, new_size)).first;
  }

  void Font::SetSize(uint32_t size)
  {
    if (size == 0) {
      utility::logger->warn("Cannot set a font size of 0!");

      return;
    }

    FontSizeList::const_iterator findSize = size_list_.find(size);

    if (findSize == size_list_.end())
    {
      AddSize(size);
    }
    else
    {
      if (utility::FTLog(FT_Activate_Size(findSize->second)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to activate font size for font \"{}\".", name_);
      }
    }

    if (utility::FTLog(FT_Select_Charmap(face_, FT_ENCODING_UNICODE)))
    {
      utility::logger->error("Failed to select Unicode encoding for font \"{}\".", name_);
    }
  }

  void Font::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Filepath: %s", name_.c_str());
      ImGui::Text("Character Count: %i", character_count_);
      
      ImGui::SeparatorText("Texture");
      ImGui::Text("ID: %i", texture_->texture_id_);
      ImGui::Text("Index: %i", texture_->texture_unit_);
      ImGui::Text("Size: %i, %i", texture_->size_.x, texture_->size_.y);
      
      ImGui::TreePop();
    }
  }

  Font* LoadFont(const std::string& filepath)
  {
    const std::filesystem::path find_path = system::SearchForResourcePath(filepath);

    if (find_path.empty())
    {
      utility::logger->warn("No font filepath given to load!");

      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(find_path.string());

    if (it != resource_list.end() && it->second->GetResourceType() != std::string(FONT_TYPE))
    {
      utility::logger->warn("\"{}\" is the name of a(n) {} resource.", find_path.string(), it->second->GetResourceType());
    }
    else if (it == resource_list.end())
    {
      FT_Face new_face = nullptr;

      if (utility::FTLog(FT_New_Face(utility::font_library, find_path.c_str(), 0, &new_face)) != FT_Err_Ok)
      {
        utility::logger->error("Failed to load face for font \"{}\".", find_path.string());

        return nullptr;
      }

      it = resource_list.emplace(std::make_pair(find_path.string(), std::make_unique<Font>(find_path, new_face))).first;
    }

    return static_cast<Font*>(it->second.get());
  }
}
