#include <algorithm>
#include <sstream>
#include "CharacterMap.h"
#include "../utility/ConversionUtils.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/LogUtils.h"

namespace term_engine::usertypes {
  CharacterMap::CharacterMap() :
    hide_empty_characters_(true)
  {
    SetSize(DEFAULT_CHARACTER_MAP_SIZE);
  }

  CharacterMap::CharacterMap(const glm::ivec2& size, const sol::table& data) :
    hide_empty_characters_(true)
  {
    SetSize(size);
    SetData(data);
  }

  glm::ivec2& CharacterMap::GetSize()
  {
    return size_;
  }

  CharacterData& CharacterMap::GetData()
  {
    return sol::as_container(data_);
  }

  bool& CharacterMap::AreEmptyCharactersHidden()
  {
    return hide_empty_characters_;
  }

  void CharacterMap::SetSize(const glm::ivec2& size)
  {
    if (size.x <= 0 || size.y <= 0)
    {
      utility::logger->warn("Cannot set character map size to 0 or less!");

      return;
    }

    const uint64_t data_size = size.x * size.y;
    data_.clear();
    data_.reserve(data_size);
    data_.resize(data_size);
    data_.shrink_to_fit();

    size_ = size;
  }

  void CharacterMap::SetData(const sol::table& data)
  {
    try
    {
      CharacterData characters = data.as<CharacterData>();

      const int in_size = characters.size();
      const int size = data_.size();

      if (in_size < size) {
        utility::logger->debug("Character data of smaller than character map. Old data will be blanked.");
        Clear();
      }
      else if (in_size > size) {
        utility::logger->debug("Character data is larger than character map. Some data will be omitted.");
      }

      for (int i = 0; i < size; ++i)
      {
        if (i >= in_size)
        {
          break;
        }

        data_[i] = characters.at(i);
      }
    }
    catch (const std::exception& err)
    {
      utility::logger->warn("Invalid data given to animation frame!");
    }
  }

  void CharacterMap::SetHideEmptyCharacters(bool flag)
  {
    hide_empty_characters_ = flag;
  }

  void CharacterMap::Clear()
  {
    std::fill(data_.begin(), data_.end(), Character());
  }

  void CharacterMap::SetFunction(const sol::function& func)
  {
    int index = 1;

    for (Character& character : data_)
    {
      character = func.call<Character>(data_, index++);
    }
  }

  void CharacterMap::PushCharacters(const glm::ivec2& position, const CharacterMap& data)
  {
    // Do not push the character map if it is outside the target character map.
    // I.e. If the top-left corner of the source character map is beyond the right/bottom edges of the target character map,
    // or if the bottom-right corner of the source character map is beyond the top/left edges of the target character map.
    if (glm::any(glm::lessThan(position + data.size_, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(position, size_)))
    {
      return;
    }

    uint64_t column_pos = 0;
    uint64_t index = utility::GetIndexFromRowCol(size_, position);

    for (const Character& character : data.data_)
    {
      bool omitEmptyChars = (character.character_ == NO_CHARACTER && data.hide_empty_characters_);

      // Do not push the character if it is an empty character.
      if (!omitEmptyChars && position.x + column_pos >= 0 && position.x + column_pos < size_.x) {
        // Do not push the character if it is outside the target character map.
        if (index >= 0 && index < data_.size()) {
          data_.at(index) = Character(character);
        }
      }

      ++column_pos;
      ++index;

      if (column_pos == data.size_.x)
      {
        index += size_.x - column_pos;
        column_pos = 0;
      }
    }
  }

  void CharacterMap::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode("Character Map"))
    {
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Hide empty characters?: %s", hide_empty_characters_ ? "Yes" : "No");
      ImGui::SeparatorText("First Char");
      ImGui::Text("Value: %i", data_.at(0).character_);
      ImGui::Text("Hidden?: %s", data_.at(0).character_ == NO_CHARACTER && hide_empty_characters_ ? "Yes" : "No");

      UpdateCharacterDataDebugInfo(data_, size_);
      
      ImGui::TreePop();
    }
  }

  void CharacterMap::CopyToBuffer(CharacterMap* character_map, const glm::ivec2& position, rendering::Buffer& buffer, Font* font_, uint32_t font_size)
  {
    uint64_t index = 0;

    for (const Character& character : character_map->data_)
    {
      const CharacterBB textBbox = font_->GetCharacter(character.character_, font_size);
      const glm::vec2 charSize = glm::vec2(font_->GetCharacterSize(font_size));
      const glm::vec2 charOffset = glm::vec2(position + utility::GetRowColFromIndex(character_map->size_, index++) * font_->GetCharacterSize(font_size));
      const glm::vec2 textOffset = glm::vec2((charSize.x - textBbox.character_size_.x) / 2, textBbox.baseline_);
      const glm::vec2 textTexPos = glm::vec2(textBbox.position_) / (float)TEXTURE_SIZE;
      const glm::vec2 textTexSize = glm::vec2(textBbox.character_size_) / (float)TEXTURE_SIZE;
      const glm::vec2 bgTexSize = glm::vec2(whitespace_bbox.character_size_) / (float)TEXTURE_SIZE;
      const glm::vec4 bgColour = (character.character_ == NO_CHARACTER && character_map->hide_empty_characters_) ? glm::vec4(0.0f) : character.background_colour_;

      // Draw the background.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(rendering::BufferData(charOffset, glm::vec2(), bgColour));
      buffer.data.push_back(rendering::BufferData(charOffset + charSize, bgTexSize, bgColour));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(0.0f, charSize.y), glm::vec2(0.0f, bgTexSize.y), bgColour));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(rendering::BufferData(charOffset, glm::vec2(), bgColour));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(charSize.x, 0), glm::vec2(bgTexSize.x, 0.0f), bgColour));
      buffer.data.push_back(rendering::BufferData(charOffset + charSize, bgTexSize, bgColour));

      // Draw the foreground.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(rendering::BufferData(charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(0, textBbox.character_size_.y) + textOffset, glm::vec2(textTexPos.x, textTexPos.y + textTexSize.y), character.foreground_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(rendering::BufferData(charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(textBbox.character_size_.x, 0) + textOffset, glm::vec2(textTexPos.x + textTexSize.x, textTexPos.y), character.foreground_colour_));
      buffer.data.push_back(rendering::BufferData(charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
    }
  }
}
