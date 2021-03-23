/// @author James Holtom

#ifndef GLYPH_SET_H
#define GLYPH_SET_H

#include <memory>
#include <string>
#include <vector>

#include "Glyph.h"
#include "../fonts/FontAtlas.h"
#include "../shaders/Shader.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  class GlyphSet;

  /// Denotes the maximum number of glyphs that can be stored in the glyph set.
  constexpr int MAX_GLYPHS = 16384;
  /// Denotes the default amount of columns in a glyph set.
  constexpr int DEFAULT_SET_WIDTH = 32;
  /// Denotes the default amount of rows in a glyph set.
  constexpr int DEFAULT_SET_HEIGHT = 16;

  /// Represents the results of calling _glyphs::GetGlyph_. The first property denotes if the call was successful, and the second is the returned _GlyphData_.
  typedef std::pair<bool, GlyphData> GetGlyphResult;
  /// Smart pointer to share a single instance of a _GlyphSet_.
  typedef std::shared_ptr<GlyphSet> GlyphSetPtr;

  /**
   * Represents the set of glyphs that're drawn to a _Scene_.
   * 
   * _TODO_: Add more description about _GlyphSet_, including a diagram of how the set is represented.
   */
  class GlyphSet {
  public:
    /// Constructs the glyph set.
    GlyphSet();

    /// Destroys the glyph set.
    ~GlyphSet();

    /// Returns the number of rows & columns in the set.
    /** 
     * @returns The size of the set.
     */
    glm::uvec2 GetSize() const;

    /// Returns the list of data that represents the set.
    /**
     * @returns The set data.
     */
    GlyphList& GetData();

    /// Sets the size of the set.
    /** 
     * @param[in] size The new size of the set.
     */
    void SetSize(const glm::uvec2& size);

    /// Returns the number of glyphs in the set.
    /** 
     * @returns The number of glyphs in the set.
     */
    size_t GetCount() const;

    /// Returns data for the glyph at the given index.
    /**
     * If the coordinates are out-of-bounds, the first glyph in the set is returned.
     */
    GetGlyphResult GetGlyph(const glm::uvec2& index);

    /// Clears all characters & colors from the set.
    void ClearAll();

    /// Resets the positions of all glyphs in the set.
    void ResetAllPositions();

    /// Logs the contents of the set.
    void PrintData() const;

    // Checks if the set is dirty (i.e. there are changes in the set that haven't been pushed to the GPU).
    /**
     * @returns If the set is dirty, and has unpushed changes.
     */
    bool IsDirty() const;

    /// Marks the set as dirty (i.e. there are changes in the set that haven't been pushed to the GPU).
    void Dirty();

    /// Renders the set to the window.
    /**
     * @param[in] font The font atlas to render the glyphs with.
     * @param[in] shader The shader to use.
     */
    void Render(const fonts::FontAtlasPtr& font, const shaders::ShaderPtr& shader);

  private:
    /// Has the set changed since it was last rendered?
    bool is_dirty_;
    /// The _Vertex Array Object_ ID of the VAO used to contain both the quad's VBO, as well as the set's VBO.
    GLuint vao_id_;
    /// The _Vertex Buffer Object_ ID of the VBO used to store the glyph's _BufferData_.
    GLuint vbo_id_;
    /// The size of the glyph set, in terms of rows/columns.
    glm::uvec2 set_size_;
    /// The glyph data, in a format that can be modified within the program and sent to the set's VBO.
    GlyphList set_data_;
    /// The OpenGL buffer data to copy to the GPU.
    BufferList buffer_data_;

    /// Prepares the internal data ready for use.
    void InitData();

    /// Prepares the OpenGL buffers ready for use.
    void InitBuffers();

    /// Copies the internal data over to the OpenGL buffers.
    /**
     * @param[in] font The font to get the texture layers from.
     */
    void SetGLBuffer(const fonts::FontAtlasPtr& font);

    /// Logs the buffer data that is passed to the GPU.
    void PrintBuffer() const;
  };
}

#endif // ! GLYPH_SET_H
