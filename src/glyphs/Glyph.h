/// @author James Holtom

#ifndef GLYPH_H
#define GLYPH_H

#include <vector>

#include "../logging/Logger.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  struct GlyphParams;
  struct BufferData;

  constexpr char NO_CHARACTER = (char)0;

  /// Used to store data in a VBO.
  typedef std::vector<BufferData> BufferList;

  /// Used to construct _GlyphData_ objects on creation, and to pass glyph data around.
  struct GlyphParams {
    /// Constructs the glyph parameters.
    /**
     * @param[in] character        The character that the glyph represents.
     * @param[in] foreground_color The foreground color, used for the text.
     * @param[in] background_color The background color.
     */
    GlyphParams(const char& character = NO_CHARACTER, const glm::vec3& foreground_color = glm::vec3(0.0f), const glm::vec3& background_color = glm::vec3(0.0f));

    /// The character that the glyph represents.
    char character_;
    /// The foreground color, used for the text.
    glm::vec3 foreground_color_;
    /// The background color.
    glm::vec3 background_color_;
  };

  /// Represents the glyph data that is passed to the GPU.
  /**
   * These are converted from _GlyphData_ objects when setting the VBO data.
   */
  struct BufferData {
    /// Constructs the buffer data.
    BufferData();

    /// Constructs the buffer data with the given parameters.
    /**
     * @param[in] texture_layer    The texture layer to render.
     * @param[in] position         The position of the glyph, from its default position in the glyph set.
     * @param[in] foreground_color The foreground color, used for the glyph.
     * @param[in] background_color The background color.
     */
    BufferData(const GLfloat& texture_layer, const glm::vec2& position, const glm::vec3& foreground_color, const glm::vec3& background_color);

    void Set(const GlyphParams& params, const bool& normalised = false);

    void SetPosition(const glm::vec2& position);

    /// Allows _std::stringstream_ to correctly parse a _InstanceData_ object.
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
    /// The position of the glyph, from its default position in the glyph set.
    glm::vec2 position_;
    /// The foreground color, used for the glyph.
    glm::vec3 foreground_color_;
    /// The background color.
    glm::vec3 background_color_;
  };

  /// Prepares the OpenGL buffers ready for use.
  int Init();
  void CleanUp();
  void Render();

  /// The _Vertex Array Object_ ID of the VAO used to contain both the quad's VBO, as well as the set's VBO.
  extern GLuint vao_id;
  /// The _Vertex Buffer Object_ ID of the VBO used to store the glyph's _BufferData_.
  extern GLuint vbo_id;
  extern glm::ivec2 dimensions;
  extern BufferList data;
  extern GlyphParams default_glyph;
}

#endif // ! GLYPH_H
