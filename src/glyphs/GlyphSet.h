/// @author James Holtom

#ifndef GLYPH_SET_H
#define GLYPH_SET_H

#include <memory>
#include <string>

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

  /// 
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
    /// Do not allow glyph sets to be constructed without setting a font atlas and shader.
    GlyphSet() = delete;

    /// Constructs the glyph set.
    /** 
     * @param[in] font_atlas The font atlas to use when drawing glyphs.
     * @param[in] shader     The shader to use for rendering glyphs.
     */
    GlyphSet(const fonts::FontAtlasPtr& font_atlas, const shaders::ShaderPtr& shader);

    /// Destroys the glyph set.
    ~GlyphSet();

    /// Returns the position of the glyph set, relative to the top-left of the window.
    /** 
     * @returns The position of the glyph set.
     */
    glm::vec2 GetPosition() const;
    
    /// Returns the size of the padding around the glyphs.
    /** 
     * @returns The size of the glyph padding.
     */
    glm::vec2 GetPadding() const;

    /// Returns the size of the spacing between glyphs.
    /** 
     * @returns The size of the glyph spacing.
     */
    glm::vec2 GetSpacing() const;

    /// Returns the number of rows & columns in the set.
    /** 
     * @returns The size of the set.
     */
    glm::uvec2 GetSize() const;

    /// 
    /**
     * 
     */
    GlyphList& GetData();

    /// Returns the filepath of the font currently being used.
    /** 
     * @returns The currently used font's filepath.
     */
    std::string GetFontPath() const;

    /// Sets the position of the set, relative to the top-left of the window.
    /** 
     * @param[in] position The new position to move the set to.
     */
    void SetPosition(const glm::vec2& position);

    /// Sets the size of the padding around the glyphs.
    /** 
     * @param[in] padding The new padding size to use.
     */
    void SetPadding(const glm::vec2& padding);

    /// Sets the size of the spacing between glyphs.
    /** 
     * @param[in] spacing The new spacing size to use.
     */
    void SetSpacing(const glm::vec2& spacing);

    /// Sets the size of the set.
    /** 
     * @param[in] size The new size of the set.
     */
    void SetSize(const glm::uvec2& size);

    /// Sets the font to use when drawing glyphs.
    /** 
     * @param[in] name The name of the font, in _font_atlas_list_, to use.
     * @see term_engine::fonts::font_atlas_list
     */
    void SetFont(const std::string& name);

    /// Sets the font to use when drawing glyphs.
    /** 
     * @param[in] font Pointer to the font to use.
     */
    void SetFont(const fonts::FontAtlasPtr& font);

    /// Sets the shader to use when rendering glyphs.
    /** 
     * @param[in] name The name of the shader, in _shader_list_, to use.
     * @see term_engine::shaders::shader_list
     */
    void SetShader(const std::string& name);

    /// Sets the shader to use when rendering glyphs.
    /** 
     * @param[in] shader Pointer to the shader to use.
     */
    void SetShader(const shaders::ShaderPtr& shader);

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

    /// Resets the offsets of all glyphs in the set.
    void ResetAllOffsets();

    /// Sets the scale of all glyphs in the set.
    /**
     * @param[in] scale The new glyph scale to use.
     */
    void SetAllScale(const glm::vec2& scale);

    /// Resets the scale of all glyphs in the set.
    void ResetAllScale();

    /// Resets the projection matrix that handles the conversion between window coordinates and OpenGL coordinates.
    void ResetProjection();

    /// Logs the contents of the set.
    void PrintData() const;

    bool IsDirty() const;

    /// Marks the set as dirty (i.e. there are changes in the set that haven't been pushed to the GPU).
    void Dirty();

    /// Renders the set to the window.
    void Render();

  private:
    /// Has the set changed since it was last rendered?
    bool is_dirty_;
    /// The _Vertex Array Object_ ID of the VAO used to contain both the quad's VBO, as well as the set's VBO.
    GLuint vao_id_;
    /// The _Vertex Buffer Object_ ID of the VBO used to store the glyph's _BufferData_.
    GLuint vbo_id_;
    /// The amount of padding around a glyph.
    /**
     * The padding space counts as part of the glyph, and the background color will appear here.
     */
    glm::vec2 glyph_padding_;
    /// The amount of spacing between glyphs.
    /**
     * The spacing does not count as part of a glyph, and the window's color will appear here.
     */
    glm::vec2 glyph_spacing_;
    /// The position of the set, relative to the window.
    glm::vec2 set_position_;
    /// The size of the glyph set, in terms of rows/columns.
    glm::uvec2 set_size_;
    /// The glyph data, in a format that can be modified within the program and sent to the set's VBO.
    GlyphList set_data_;
    /// The font atlas to use when rendering the glyphs.
    fonts::FontAtlasPtr set_font_;
    /// The shader to use when rendering the glyphs.
    shaders::ShaderPtr set_shader_;

    /// Prepares the internal data ready for use.
    void InitData();

    /// Prepares the OpenGL buffers ready for use.
    void InitBuffers();

    /// Copies the internal data over to the OpenGL buffers.
    void SetGLBuffer() const;

    /// Retrieves the original position of the given glyph, relative to the window.
    /**
     * @param[in] glyph The glyph to get the position for.
     * @returns The original position of the glyph, relative to the window.
     */
    glm::vec2 GetGlyphOrigin(const GlyphData& glyph) const;

    /// Logs the buffer data that is passed to the GPU.
    /**
     * @param[in] data The buffer data to log.
     */
    void PrintBuffer(const BufferList& data) const;
  };
}

#endif // ! GLYPH_SET_H
