/// @author James Holtom

#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include "../logging/Logger.h"
#include "../utility/GLUtils.h"

namespace term_engine::objects {
  struct CharacterParams;
  struct BufferData;

  /// @brief Used to store a collection of character parameters, which represents an object.
  typedef std::vector<CharacterParams> CharacterData;
  /// @brief Used to store data in a VBO.
  typedef std::vector<BufferData> BufferList;

  /// @brief Used to indicate no character is set.
  constexpr char NO_CHARACTER = '\0';
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
    CharacterParams(const char& character = NO_CHARACTER, const glm::vec4& foreground_colour = DEFAULT_FOREGROUND_COLOUR, const glm::vec4& background_colour = DEFAULT_BACKGROUND_COLOUR) :
      character_(character),
      foreground_colour_(foreground_colour),
      background_colour_(background_colour) {}

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
    char character_;
    /// @brief The foreground colour, used for the text.
    glm::vec4 foreground_colour_;
    /// @brief The background colour.
    glm::vec4 background_colour_;
  };

  /// @brief Represents the structure of the buffer used to render characters.
  struct BufferData {
    /// @brief Constructs the buffer data.
    BufferData() = default;

    /**
     * @brief Constructs the buffer data with the given parameters.
     * 
     * @param[in] position          The position of the vertex.
     * @param[in] texture_position  The position of the texture.
     * @param[in] has_texture       Does the current vertex have a texture?
     * @param[in] colour            The colour to render the vertex/texture with.
     */
    BufferData(const glm::vec2& position, const glm::vec2& texture_position, const bool& has_texture, const glm::vec4& colour) :
      position_(position),
      texture_position_(texture_position),
      has_texture_(has_texture ? 1.0f : 0.0f),
      colour_(colour / 255.0f) {}

    /**
     * @brief Allows _std::stringstream_ to correctly parse a _BufferData_ object.
     * 
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Vertex position: " << data.position_.x << ", " << data.position_.y << std::endl
        << "Texture position: " << data.texture_position_.x << ", " << data.texture_position_.y << std::endl
        << "Colour: " << data.colour_.r << ", " << data.colour_.g << ", " << data.colour_.b << ", " << data.colour_.a << std::endl;
    }

    /// @brief The position of the vertex.
    glm::vec2 position_;
    /// @brief Does the current vertex have a texture?
    GLfloat has_texture_;
    /// @brief The position of the texture.
    glm::vec2 texture_position_;
    /// @brief The colour to render the vertex/texture with.
    glm::vec4 colour_;
  };
}

#endif // ! CHARACTER_H
