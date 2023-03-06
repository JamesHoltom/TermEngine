#include <algorithm>
#include <sstream>
#include "CharacterMap.h"
#include "../utility/DebugUtils.h"
#include "../utility/IndexUtils.h"

namespace term_engine::rendering {
  // TODO: Set default character size here!
  CharacterMap::CharacterMap() :
    position_(glm::vec2()),
    size_(DEFAULT_CHARACTER_MAP_SIZE),
    font_size_(DEFAULT_FONT_SIZE)
  {
    const size_t size = size_.x * size_.y;

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

  GLuint CharacterMap::GetFontSize() const
  {
    return font_size_;
  }

  void CharacterMap::SetPosition(const glm::vec2& position)
  {
    position_ = position;
  }

  void CharacterMap::SetSize(const glm::ivec2& size)
  {
    data_.clear();
    data_.reserve((size_t)size.x * (size_t)size.y);
    data_.resize((size_t)size.x * (size_t)size.y);

    size_ = size;
  }

  void CharacterMap::SetFontSize(const GLuint& font_size)
  {
    font_size_ = font_size;
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

  void CharacterMap::CopyToBuffer(Buffer& buffer, const FontAtlasPtr& font_atlas) const
  {
    size_t index = 0;
    BufferList text_buffer;
    text_buffer.reserve(data_.size() * 6);

    for (const CharacterParams& character : data_)
    {
      const CharacterBB textBbox = font_atlas->GetCharacter(character.character_, font_size_);
      const CharacterBB bgBbox = font_atlas->GetWhitespace();
      const glm::vec2 charSize = glm::vec2(font_atlas->GetCharacterSize(font_size_));
      const glm::vec2 charOffset = glm::vec2(utility::GetRowColFromIndex(size_, index++) * font_atlas->GetCharacterSize(font_size_));
      const glm::vec2 textOffset = glm::vec2((charSize.x - textBbox.character_size_.x) / 2, textBbox.baseline_);
      const glm::vec2 textTexPos = glm::vec2(textBbox.position_) / (GLfloat)TEXTURE_SIZE;
      const glm::vec2 textTexSize = glm::vec2(textBbox.character_size_) / (GLfloat)TEXTURE_SIZE;
      const glm::vec2 bgTexSize = glm::vec2(bgBbox.character_size_) / (GLfloat)TEXTURE_SIZE;

      // Draw the background.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      buffer.data.push_back(BufferData(position_ + charOffset, glm::vec2(), character.background_colour_));
      buffer.data.push_back(BufferData(position_ + charOffset + charSize, bgTexSize, character.background_colour_));
      buffer.data.push_back(BufferData(position_ + charOffset + glm::vec2(0.0f, charSize.y), glm::vec2(0.0f, bgTexSize.y), character.background_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      buffer.data.push_back(BufferData(position_ + charOffset, glm::vec2(), character.background_colour_));
      buffer.data.push_back(BufferData(position_ + charOffset + glm::vec2(charSize.x, 0), glm::vec2(bgTexSize.x, 0.0f), character.background_colour_));
      buffer.data.push_back(BufferData(position_ + charOffset + charSize, bgTexSize, character.background_colour_));

      // Draw the foreground.
      /* Draw order:
       * 1
       * |\
       * | \
       * 3--2
       */
      text_buffer.push_back(BufferData(position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + charOffset + glm::vec2(0, textBbox.character_size_.y) + textOffset, glm::vec2(textTexPos.x, textTexPos.y + textTexSize.y), character.foreground_colour_));

      /* Draw order:
       * 1--2
       *  \ |
       *   \|
       *    3
       */
      text_buffer.push_back(BufferData(position_ + charOffset + textOffset, textTexPos, character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + charOffset + glm::vec2(textBbox.character_size_.x, 0) + textOffset, glm::vec2(textTexPos.x + textTexSize.x, textTexPos.y), character.foreground_colour_));
      text_buffer.push_back(BufferData(position_ + charOffset + glm::vec2(textBbox.character_size_) + textOffset, textTexPos + textTexSize, character.foreground_colour_));
    }

    buffer.data.insert(buffer.data.end(), text_buffer.begin(), text_buffer.end());
  }
}
