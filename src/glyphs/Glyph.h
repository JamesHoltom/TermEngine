/// @author James Holtom

#ifndef GLYPH_H
#define GLYPH_H

#include <ostream>
#include <vector>

#include "../utility/LoggingUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  struct BufferData;
  struct GlyphData;

  /// The default foreground color (i.e. text color) to use for glyphs.
  constexpr glm::vec4 DEFAULT_FOREGROUND_COLOR = glm::vec4(1.0f);
  /// The default background color to use for glyphs.
  constexpr glm::vec4 DEFAULT_BACKGROUND_COLOR = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  /// Used to store a set of glyphs.
  typedef std::vector<GlyphData> GlyphList;
  /// Used to store data in a VBO.
  typedef std::vector<BufferData> BufferList;

  /// Represents the glyph data that is passed to the GPU.
  /**
   * These are converted from _GlyphData_ objects when setting the VBO data.
   */
  struct BufferData {
    /// The texture layer to render.
    GLfloat texture_layer_;
    /// The position of the glyph, relative to the containing _GlyphSet_.
    glm::vec2 offset_;
    /// The size of the glyph.
    glm::vec2 scale_;
    /// The foreground color, used for the glyph.
    glm::vec4 foreground_color_;
    /// The background color.
    glm::vec4 background_color_;

    /// Constructs the buffer data.
    BufferData();

    /// Constructs the buffer data with the given parameters.
    /**
     * @param[in] texture_layer    The texture layer to render.
     * @param[in] offset           The position of the glyph, relative to the containing _GlyphSet_.
     * @param[in] scale            The size of the glyph.
     * @param[in] foreground_color The foreground color, used for the glyph.
     * @param[in] background_color The background color.
     */
    BufferData(const GLfloat& texture_layer, const glm::vec2& offset, const glm::vec2& scale, const glm::vec4& foreground_color, const glm::vec4& background_color);

    /// Allows _std::stringstream_ to correctly parse a _InstanceData_ object.
    /**
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Texture layer: " << data.texture_layer_ << std::endl
        << "Glyph offset: " << data.offset_ << std::endl
        << "Glyph scale: " << data.scale_ << std::endl
        << "Foreground colour: " << data.foreground_color_ << std::endl
        << "Background colour: " << data.background_color_ << std::endl;
    }
  };

  /// Used to construct _GlyphData_ objects on creation, and to pass glyph data around.
  struct GlyphParams {
    /// Constructs the glyph parameters.
    /**
     * @param[in] character        The character that the glyph represents.
     * @param[in] offset           The position of the glyph, relative to the glyph's origin in the containing _GlyphSet_.
     * @param[in] scale            The size of the glyph.
     * @param[in] foreground_color The foreground color, used for the text.
     * @param[in] background_color The background color.
     */
    GlyphParams(const char& character, const glm::vec2& offset, const glm::vec2& scale, const glm::vec4& foreground_color, const glm::vec4& background_color);

    /// The character that the glyph represents.
    char character_;
    /// The position of the glyph, relative to the glyph's origin in the containing _GlyphSet_.
    glm::vec2 offset_;
    /// The size of the glyph.
    glm::vec2 scale_;
    /// The foreground color, used for the text.
    glm::vec4 foreground_color_;
    /// The background color.
    glm::vec4 background_color_;
  };

  /// Used to represent glyphs within a _GlyphSet_.
  /**
   * Unlike _GlyphParams_, these objects have a fixed position within a _GlyphSet_, which cannot be changed after creation.
   */
  struct GlyphData {
    /// Do not allow _GlyphData_ objects to be constructed without parameters. The index, which relates to it's position within a _GlyphSet_, is required.
    GlyphData() = delete;

    /// Constructs the glyph data with the given index.
    /**
     * @param[in] index The position of the glyph within the _GlyphSet_.
     */
    GlyphData(const glm::uvec2& index);

    /// Constructs the glyph data with the given index and parameters.
    /**
     * @param[in] index The position of the glyph within the _GlyphSet_.
     * @param[in] glyph The parameters to construct the glyph with.
     */
    GlyphData(const glm::uvec2& index, const GlyphParams& glyph);

    /// Sets the glyph's data with the given parameters.
    /**
     * @param[in] glyph The parameters to apply to the glyph.
     */
    void SetParams(const GlyphParams& glyph);

    /**
     * Clears the data, returning the object to default values.
     */
    void Clear();

    /// Allows _std::stringstream_ to correctly parse a _GlyphData_ object.
    /**
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const GlyphData& data) {
      return os << std::endl
        << "Position:          " << data.index_ << std::endl
        << "Offset:            " << data.offset_ << std::endl
        << "Scale:             " << data.scale_<< std::endl
        << "Character:         " << data.character_ << std::endl
        << "Foreground colour: " << data.foreground_color_ << std::endl
        << "Background colour: " << data.background_color_ << std::endl;
    }

    /// The position of the glyph within the _GlyphSet_.
    const glm::uvec2 index_;
    /// The character that the glyph represents.
    char character_;
    /// The position of the glyph, relative to the glyph's origin in the containing _GlyphSet_.
    glm::vec2 offset_;
    /// The size of the glyph.
    glm::vec2 scale_;
    /// The foreground color, used for the text.
    glm::vec4 foreground_color_;
    /// The background color.
    glm::vec4 background_color_;
  };
}

#endif // ! GLYPH_H
