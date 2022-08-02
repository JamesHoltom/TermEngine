/// @author James Holtom

#ifndef GLYPH_H
#define GLYPH_H

#include <vector>
#include "../logging/Logger.h"
#include "../utility/GLUtils.h"

namespace term_engine::objects {
  struct GlyphParams;
  struct BufferData;

  /// Used to store a collection of glyph parameters, which represents an object.
  typedef std::vector<GlyphParams> GlyphData;

  /// Used to indicate no character is set.
  constexpr char NO_CHARACTER = '\0';
  /// The default foreground colour for glyphs.
  constexpr glm::vec3 DEFAULT_FOREGROUND_COLOUR = glm::vec3(255.0f);
  /// The default background colour for glyphs.
  constexpr glm::vec3 DEFAULT_BACKGROUND_COLOUR = glm::vec3(0.0f);

  /// Used to store data in a VBO.
  typedef std::vector<BufferData> BufferList;

  /// Used to construct _BufferData_ objects, and to pass data between scripts and the program.
  struct GlyphParams {
    /// Constructs the glyph parameters.
    /**
     * @param[in] character        The character that the glyph represents.
     * @param[in] foreground_colour The foreground colour, used for the text.
     * @param[in] background_colour The background colour.
     */
    GlyphParams(const char& character = NO_CHARACTER, const glm::vec3& foreground_colour = DEFAULT_FOREGROUND_COLOUR, const glm::vec3& background_colour = DEFAULT_BACKGROUND_COLOUR) :
      character_(character),
      foreground_colour_(foreground_colour),
      background_colour_(background_colour) {}

    /// Allows for comparing 2 sets of _GlyphParams_ objects.
    /**
     * @param[in] lhs The left-hand side object.
     * @param[in] rhs The right-hand side object.
     * @returns If the 2 objects have equal values.
     */
    friend bool operator== (const GlyphParams& lhs, const GlyphParams& rhs)
    {
      return (lhs.character_ == rhs.character_) && (lhs.foreground_colour_ == rhs.foreground_colour_) && (lhs.background_colour_ == rhs.background_colour_);
    }

    /// The character that the glyph represents.
    char character_;
    /// The foreground colour, used for the text.
    glm::vec3 foreground_colour_;
    /// The background colour.
    glm::vec3 background_colour_;
  };

  /// Represents the structure of the buffer used to render glyphs.
  struct BufferData {
    /// Constructs the buffer data.
    BufferData() {}

    /// Constructs the buffer data with the given parameters.
    /**
     * @param[in] position          The position of the vertex.
     * @param[in] texture_position  The position of the texture.
     * @param[in] has_texture       Does the current vertex have a texture?
     * @param[in] colour             The colour to render the vertex/texture with.
     */
    BufferData(const glm::vec2& position, const glm::vec2& texture_position, const bool& has_texture, const glm::vec3& colour) :
      position_(position),
      texture_position_(texture_position),
      has_texture_(has_texture ? 1.0f : 0.0f),
      colour_(colour / 255.0f) {}

    /// Allows _std::stringstream_ to correctly parse a _BufferData_ object.
    /**
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Vertex position: " << data.position_.x << ", " << data.position_.y << std::endl
        << "Texture position: " << data.texture_position_.x << ", " << data.texture_position_.y << std::endl
        << "Colour: " << data.colour_.r << ", " << data.colour_.g << ", " << data.colour_.b << std::endl;
    }

    /// The position of the vertex.
    glm::vec2 position_;
    /// Does the current vertex have a texture?
    GLfloat has_texture_;
    /// The position of the texture.
    glm::vec2 texture_position_;
    /// The colour to render the vertex/texture with.
    glm::vec3 colour_;
  };
}

#endif // ! GLYPH_H
