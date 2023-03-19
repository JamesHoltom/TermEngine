#include <algorithm>
#include <sstream>
#include "CharacterMap.h"
#include "../utility/DebugUtils.h"
#include "../utility/IndexUtils.h"

namespace term_engine::rendering {
  CharacterMap::CharacterMap() :
    position_(glm::vec2()),
    size_(DEFAULT_CHARACTER_MAP_SIZE)
  {
    const uint64_t size = size_.x * size_.y;

    data_.reserve(size);
    data_.resize(size);
  }

  glm::vec2 CharacterMap::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 CharacterMap::GetSize() const
  {
    return size_;
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

  void CopyCharacterMapToBuffer(const CharacterMap& character_map, Buffer& buffer, const FontAtlasPtr& font_atlas, uint32_t font_size)
  {
    uint64_t index = 0;

    for (const CharacterParams& character : character_map.data_)
    {
      const CharacterBB textBbox = font_atlas->GetCharacter(character.character_, font_size);
      const CharacterBB bgBbox = font_atlas->GetWhitespace();
      const glm::vec2 charSize = glm::vec2(font_atlas->GetCharacterSize(font_size));
      const glm::vec2 charOffset = glm::vec2(utility::GetRowColFromIndex(character_map.size_, index++) * font_atlas->GetCharacterSize(font_size));
      const glm::vec2 textOffset = glm::vec2((charSize.x - textBbox.character_size_.x) / 2, textBbox.baseline_);
      const glm::vec2 textTexPos = glm::vec2(textBbox.position_) / (float)TEXTURE_SIZE;
      const glm::vec2 textTexSize = glm::vec2(textBbox.character_size_) / (float)TEXTURE_SIZE;
      const glm::vec2 bgTexSize = glm::vec2(bgBbox.character_size_) / (float)TEXTURE_SIZE;

      // Draw the background.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(BufferData(character_map.position_ + charOffset, glm::vec2(), character.background_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + charSize, bgTexSize, character.background_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(0.0f, charSize.y), glm::vec2(0.0f, bgTexSize.y), character.background_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(BufferData(character_map.position_ + charOffset, glm::vec2(), character.background_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(charSize.x, 0), glm::vec2(bgTexSize.x, 0.0f), character.background_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + charSize, bgTexSize, character.background_colour_));

      // Draw the foreground.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(0, textBbox.character_size_.y) + textOffset, glm::vec2(textTexPos.x, textTexPos.y + textTexSize.y), character.foreground_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_.x, 0) + textOffset, glm::vec2(textTexPos.x + textTexSize.x, textTexPos.y), character.foreground_colour_));
      buffer.data.push_back(BufferData(character_map.position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
    }
  }
}
