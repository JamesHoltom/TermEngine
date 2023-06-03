/// @author James Holtom

#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "../utility/GLUtils.h"

namespace term_engine::usertypes {
  struct CharacterParams;

  /// @brief Used to store a collection of character parameters, which represents an object.
  typedef std::vector<CharacterParams> CharacterData;

  /// @brief Used to indicate no character is set.
  constexpr char16_t NO_CHARACTER = U'\0';
  /// @brief The default foreground colour for characters.
  constexpr glm::vec4 DEFAULT_FOREGROUND_COLOUR = glm::vec4(255.0f);
  /// @brief The default background colour for characters.
  constexpr glm::vec4 DEFAULT_BACKGROUND_COLOUR = glm::vec4(0.0f, 0.0f, 0.0f, 255.0f);

  /// @brief Used to construct _BufferData_ objects, and to pass data between scripts and the program.
  struct CharacterParams {
    /**
     * @brief Constructs the character parameters.
     * 
     * @param[in] character         The character that the character represents.
     * @param[in] foreground_colour The foreground colour, used for the text.
     * @param[in] background_colour The background colour.
     */
    CharacterParams(char16_t character = NO_CHARACTER, const glm::vec4& foreground_colour = DEFAULT_FOREGROUND_COLOUR, const glm::vec4& background_colour = DEFAULT_BACKGROUND_COLOUR) :
      character_(character),
      foreground_colour_(foreground_colour),
      background_colour_(background_colour) {};

    /**
     * @brief Copy-constructs the character parameters.
     * 
     * @param[in] character The character to copy from.
     */
    CharacterParams(const CharacterParams& character) :
      character_(character.character_),
      foreground_colour_(character.foreground_colour_),
      background_colour_(character.background_colour_) {}

    /**
     * @brief Allows for comparing 2 sets of _CharacterParams_ objects.
     * 
     * @param[in] lhs The left-hand side object.
     * @param[in] rhs The right-hand side object.
     * @returns If the 2 objects have equal values.
     */
    friend bool operator== (const CharacterParams& lhs, const CharacterParams& rhs)
    {
      return (lhs.character_ == rhs.character_) && (lhs.foreground_colour_ == rhs.foreground_colour_) && (lhs.background_colour_ == rhs.background_colour_);
    }

    /// @brief The character that the character represents.
    char16_t character_;
    /// @brief The foreground colour, used for the text.
    glm::vec4 foreground_colour_;
    /// @brief The background colour.
    glm::vec4 background_colour_;
  };

  void UpdateCharacterDataDebugInfo(const CharacterData& data);
}

#endif // ! CHARACTER_H
