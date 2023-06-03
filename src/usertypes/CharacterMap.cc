#include <algorithm>
#include <sstream>
#include "CharacterMap.h"
#include "../utility/ConversionUtils.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/LogUtils.h"

namespace term_engine::usertypes {
  CharacterMap::CharacterMap() :
    position_(glm::vec2()),
    size_(DEFAULT_CHARACTER_MAP_SIZE),
    empty_character_bg_colour_(glm::vec4())
  {
    const uint64_t size = size_.x * size_.y;

    data_.reserve(size);
    data_.resize(size);
  }

  glm::vec2& CharacterMap::GetPosition()
  {
    return position_;
  }

  glm::ivec2& CharacterMap::GetSize()
  {
    return size_;
  }

  bool CharacterMap::ShouldEmptyCharactersHaveBackground() const
  {
    return empty_character_bg_colour_.a == 0.0;
  }

  void CharacterMap::SetPosition(const glm::vec2& position)
  {
    position_ = position;
  }

  void CharacterMap::SetSize(const glm::ivec2& size)
  {
    const uint64_t data_size = size.x * size.y;
    data_.clear();
    data_.reserve(data_size);
    data_.resize(data_size);

    size_ = size;
  }

  void CharacterMap::SetEmptyCharacterBackground(bool flag)
  {
    empty_character_bg_colour_.a = flag ? 0.0 : 255.0;
  }

  void CharacterMap::Clear()
  {
    std::fill(data_.begin(), data_.end(), CharacterParams());
  }

  void CharacterMap::PushCharacters(const glm::ivec2& position, const glm::ivec2& size, const CharacterData& data)
  {
    // Do not render the object if it is obscured from view.
    // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
    if (glm::any(glm::lessThan(position + size, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(position, size_)))
    {
      return;
    }

    uint64_t column_pos = 0;
    uint64_t index = utility::GetIndexFromRowCol(size_, position);

    for (const CharacterParams& character : data)
    {
      // Do not render the character if it is invisible.
      if (character.character_ != NO_CHARACTER) {
        // Do not render the character if it is obscured from view.
        if (index >= 0 && index < size_.x * size_.y) {
          data_[index] = CharacterParams(character);
        }
      }

      ++column_pos;
      ++index;

      if (column_pos == size.x)
      {
        index += size_.x - size.x;
        column_pos = 0;
      }
    }
  }

  void CharacterMap::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode("Character Map"))
    {
      ImGui::Text("Position: %f, %f", position_.x, position_.y);
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Colour: ");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(empty_character_bg_colour_.r, empty_character_bg_colour_.g, empty_character_bg_colour_.b, empty_character_bg_colour_.a), "%f, %f, %f, %f", empty_character_bg_colour_.r, empty_character_bg_colour_.g, empty_character_bg_colour_.b, empty_character_bg_colour_.a);

      UpdateCharacterDataDebugInfo(data_);
      
      ImGui::TreePop();
    }
  }

  void CopyCharacterMapToBuffer(const CharacterMap& character_map, rendering::Buffer& buffer, Font* font_, uint32_t font_size)
  {
    uint64_t index = 0;

    for (const CharacterParams& character : character_map.data_)
    {
      const CharacterBB textBbox = font_->GetCharacter(character.character_, font_size);
      const glm::vec2 charSize = glm::vec2(font_->GetCharacterSize(font_size));
      const glm::vec2 charOffset = glm::vec2(utility::GetRowColFromIndex(character_map.size_, index++) * font_->GetCharacterSize(font_size));
      const glm::vec2 textOffset = glm::vec2((charSize.x - textBbox.character_size_.x) / 2, textBbox.baseline_);
      const glm::vec2 textTexPos = glm::vec2(textBbox.position_) / (float)TEXTURE_SIZE;
      const glm::vec2 textTexSize = glm::vec2(textBbox.character_size_) / (float)TEXTURE_SIZE;
      const glm::vec2 bgTexSize = glm::vec2(whitespace_bbox.character_size_) / (float)TEXTURE_SIZE;
      const glm::vec4 bgColour = character.character_ == 0 ? character_map.empty_character_bg_colour_ : character.background_colour_;

      // Draw the background.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset, glm::vec2(), bgColour));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + charSize, bgTexSize, bgColour));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(0.0f, charSize.y), glm::vec2(0.0f, bgTexSize.y), bgColour));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset, glm::vec2(), bgColour));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(charSize.x, 0), glm::vec2(bgTexSize.x, 0.0f), bgColour));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + charSize, bgTexSize, bgColour));

      // Draw the foreground.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(0, textBbox.character_size_.y) + textOffset, glm::vec2(textTexPos.x, textTexPos.y + textTexSize.y), character.foreground_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_.x, 0) + textOffset, glm::vec2(textTexPos.x + textTexSize.x, textTexPos.y), character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
    }
  }
}
