/// @author James Holtom

#ifndef GLYPH_H
#define GLYPH_H

#include <vector>

#include "../logging/Logger.h"
#include "../resources/FontAtlas.h"
#include "../utility/GLUtils.h"

namespace term_engine {
  struct GlyphParams;
  struct BufferData;

  /// Used to indicate no character is set.
  constexpr char NO_CHARACTER = '\0';
  /// The default foreground color for glyphs.
  constexpr glm::vec3 DEFAULT_FOREGROUND_COLOR = glm::vec3(255.0f);
  /// The default background color for glyphs.
  constexpr glm::vec3 DEFAULT_BACKGROUND_COLOR = glm::vec3(0.0f);

  /// Used to store data in a VBO.
  typedef std::vector<BufferData> BufferList;

  /// Default glyph data to copy.
  extern GlyphParams default_glyph;

  /// Used to construct _BufferData_ objects, and to pass data between scripts and the program.
  struct GlyphParams {
    /// Constructs the glyph parameters.
    /**
     * @param[in] character        The character that the glyph represents.
     * @param[in] foreground_color The foreground color, used for the text.
     * @param[in] background_color The background color.
     */
    GlyphParams(const char& character = NO_CHARACTER, const glm::vec3& foreground_color = DEFAULT_FOREGROUND_COLOR, const glm::vec3& background_color = DEFAULT_BACKGROUND_COLOR) :
      character_(character),
      foreground_color_(foreground_color),
      background_color_(background_color) {}

    /// Allows for comparing 2 sets of _GlyphParams_ objects.
    /**
     * @param[in] lhs The left-hand side object.
     * @param[in] rhs The right-hand side object.
     * @returns If the 2 objects have equal values.
     */
    friend bool operator== (const GlyphParams& lhs, const GlyphParams& rhs)
    {
      return (lhs.character_ == rhs.character_) && (lhs.foreground_color_ == rhs.foreground_color_) && (lhs.background_color_ == rhs.background_color_);
    }

    /// The character that the glyph represents.
    char character_;
    /// The foreground color, used for the text.
    glm::vec3 foreground_color_;
    /// The background color.
    glm::vec3 background_color_;
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
     * @param[in] color             The color to render the vertex/texture with.
     */
    BufferData(const glm::vec2& position, const glm::vec2& texture_position, const bool& has_texture, const glm::vec3& color) :
      position_(position),
      texture_position_(texture_position),
      has_texture_(has_texture ? 1.0f : 0.0f),
      color_(color / 255.0f) {}

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
        << "Colour: " << data.color_.r << ", " << data.color_.g << ", " << data.color_.b << std::endl;
    }

    /// The position of the vertex.
    glm::vec2 position_;
    /// Does the current vertex have a texture?
    GLfloat has_texture_;
    /// The position of the texture.
    glm::vec2 texture_position_;
    /// The color to render the vertex/texture with.
    glm::vec3 color_;
  };
}

#endif // ! GLYPH_H
