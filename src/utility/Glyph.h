/// @author James Holtom

#ifndef GLYPH_H
#define GLYPH_H

#include <vector>

#include "../logging/Logger.h"
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
    BufferData() :
      texture_layer_(0.0f),
      position_(glm::vec2(0.0f)),
      foreground_color_(glm::vec4(0.0f)),
      background_color_(glm::vec4(0.0f)) {}

    /// Constructs the buffer data with the given parameters.
    /**
     * @param[in] texture_layer    The texture layer to render.
     * @param[in] position         The position of the glyph, from its default position in the glyph set.
     * @param[in] foreground_color The foreground color, used for the glyph.
     * @param[in] background_color The background color.
     */
    BufferData(const GLfloat& texture_layer, const glm::vec2& position, const glm::vec3& foreground_color, const glm::vec3& background_color) :
      texture_layer_(texture_layer),
      position_(position),
      foreground_color_(foreground_color),
      background_color_(background_color) {}

    /// Sets the parameters for the glyph.
    /**
     * @param[in] params     The parameters to apply to the glyph.
     * @param[in] normalised If the foreground and background colors are normalised (i.e. from 0~1, rather than 0~255).
     */
    void Set(const GlyphParams& params, const bool& normalised = false);

    /// Sets the position of the glyph, relative to the window.
    /**
     * @param[in] position The position to move the glyph to.
     */
    void SetPosition(const glm::vec2& position);

    /// Allows _std::stringstream_ to correctly parse a _BufferData_ object.
    /**
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Texture layer: " << data.texture_layer_ << std::endl
        << "Position: " << data.position_.x << ", " << data.position_.y << std::endl
        << "Foreground colour: " << data.foreground_color_.r << ", " << data.foreground_color_.g << ", " << data.foreground_color_.b << std::endl
        << "Background colour: " << data.background_color_.r << ", " << data.background_color_.g << ", " << data.background_color_.b << std::endl;
    }

    /// The texture layer to render.
    GLfloat texture_layer_;
    /// The position of the glyph, relative to the window.
    glm::vec2 position_;
    /// The foreground color, used for the glyph.
    glm::vec3 foreground_color_;
    /// The background color.
    glm::vec3 background_color_;
  };
}

#endif // ! GLYPH_H
