#include <sstream>
#include "CharacterMap.h"
#include "../utility/DebugUtils.h"
#include "../utility/IndexUtils.h"

namespace term_engine::rendering {
  CharacterMap::CharacterMap() :
    position_(glm::vec2()),
    size_(DEFAULT_CHARACTER_MAP_SIZE)
  {
    const size_t size = size_.x * size_.y;

    data_.reserve(size);
    data_.resize(size);
  }

  glm::vec2 CharacterMap::GetPosition()
  {
    return position_;
  }

  glm::ivec2 CharacterMap::GetSize()
  {
    return size_;
  }

  void CharacterMap::SetPosition(const glm::vec2& position)
  {
    position_ = position;
  }

  void CharacterMap::SetSize(const glm::ivec2& size)
  {
    data_.reserve((size_t)size.x * (size_t)size.y);
    data_.resize((size_t)size.x * (size_t)size.y);
    data_.shrink_to_fit();

    size_ = size;
  }

  void CharacterMap::PushCharacters(const glm::ivec2& position, const glm::ivec2& size, const CharacterData& data)
  {
    // Do not render the object if it is obscured from view.
    // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
    if (glm::any(glm::lessThan(position + size, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(position, size_)))
    {
      return;
    }

    size_t column_pos = 0;
    size_t index = utility::GetIndexFromRowCol(size_, position);

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

  void CharacterMap::CopyToBuffer(Buffer& buffer, FontAtlas& font_atlas) const
  {
    size_t index = 0;
    BufferList text_buffer;
    text_buffer.reserve(data_.size() * 6);

    for (const CharacterParams& character : data_)
    {
      const CharacterBB bbox = font_atlas.GetCharacter(character.character_);
      const glm::vec2 character_size = glm::vec2(font_atlas.GetCharacterSize());
      const glm::vec2 texPos = glm::vec2(bbox.position_) / (GLfloat)TEXTURE_SIZE;
      const glm::vec2 texSize = glm::vec2(bbox.size_) / (GLfloat)TEXTURE_SIZE;
      const glm::vec2 texAlign = glm::vec2((character_size.x - bbox.size_.x) / 2, bbox.baseline_);
      const glm::vec2 offset = glm::vec2(utility::GetPositionFromIndex(font_atlas, size_, index++));
      const bool has_texture = character.character_ != NO_CHARACTER;

      // Draw the background.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(BufferData(position_ + offset, glm::vec2(), false, character.background_colour_));
      buffer.data.push_back(BufferData(position_ + offset + character_size, glm::vec2(), false, character.background_colour_));
      buffer.data.push_back(BufferData(position_ + offset + glm::vec2(0.0f, character_size.y), glm::vec2(), false, character.background_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(BufferData(position_ + offset, glm::vec2(), false, character.background_colour_));
      buffer.data.push_back(BufferData(position_ + offset + glm::vec2(character_size.x, 0), glm::vec2(), false, character.background_colour_));
      buffer.data.push_back(BufferData(position_ + offset + character_size, glm::vec2(), false, character.background_colour_));

      // Draw the foreground.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      text_buffer.push_back(BufferData(position_ + offset + texAlign, texPos, has_texture, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + offset + glm::vec2(bbox.size_) + texAlign, texPos + texSize, has_texture, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + offset + glm::vec2(0, bbox.size_.y) + texAlign, glm::vec2(texPos.x, texPos.y + texSize.y), has_texture, character.foreground_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      text_buffer.push_back(BufferData(position_ + offset + texAlign, texPos, has_texture, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + offset + glm::vec2(bbox.size_.x, 0) + texAlign, glm::vec2(texPos.x + texSize.x, texPos.y), has_texture, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + offset + glm::vec2(bbox.size_) + texAlign, texPos + texSize, has_texture, character.foreground_colour_));
    }

    buffer.data.insert(buffer.data.end(), text_buffer.begin(), text_buffer.end());
  }
}
