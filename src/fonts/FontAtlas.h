/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::fonts {
  class FontAtlas;

  /// Used to store characters and the texture layers of the glyph they represent.
  typedef std::unordered_map<FT_UInt32, GLuint> GlyphList;
  /// Smart pointer used to share a single instance of a font atlas.
  typedef std::shared_ptr<FontAtlas> FontAtlasPtr;
  /// Used to store a list of font atlas, with the font path as the key.
  typedef std::unordered_map<std::string, FontAtlasPtr> FontAtlasList;
  /// Iterates over the elements of a _FontAtlasList_.
  typedef std::pair<std::string, FontAtlasPtr> FontAtlasIter;
  
  /// Responsible for storing and rendering glyphs for a single font and size.
  /** 
   * _TODO_: Describe the lifecycle of loading and rendering glyphs with this class.
   */
  class FontAtlas {
  public:
    /// Constructs the font atlas.
    /** 
     * @param[in] library An instance of the FreeType library.
     * @param[in] font_path A relative filepath to the font to use.
     * @param[in] glyph_size The size of the glyphs to render, in pixels (px).
     */
    FontAtlas(const FT_Library& library, const std::string& font_path, const int& glyph_size);

    /// Destroys the font atlas.
    ~FontAtlas();

    /// Retrieves the filepath of the font being used.
    /**
     * @returns The filepath to the font being used.
     */
    std::string GetFontPath() const;

    /// Returns the font size of rendered glyphs, in pixels (px).
    /** 
     * @returns The size of rendered glyphs, in pixels (px).
     */
    int GetFontSize() const;

    /// Finds a glyph in the atlas and returns the texture layer the glyph is on.
    /**
     * If no glyph is found, it is loaded from the font and stored in the atlas.
     * 
     * @param[in] character The character to look up.
     * @returns The texture layer that holds the character.
     */
    GLint GetCharacter(const FT_ULong &character);

    /// Returns the atlas' texture ID for OpenGL to use.
    /** 
     * @returns The texture ID.
     */
    const GLuint& GetTextureId() const;

    /// Returns the size of the atlas texture.
    /** 
     * @returns The size of the atlas texture.
     */
    glm::uvec2 GetTextureSize() const;

    /// Binds the atlas texture ready for OpenGL to render.
    void Use() const;

    /// Unbinds the atlas texture.
    void Unuse() const;

  private:
    /// The relative filepath of the font being used.
    std::string font_path_;
    /// The font size, in pixels (px).
    int font_size_;
    /// The face data for the last loaded glyph.
    FT_Face font_face_;
    /// The list containing all glyphs loaded from the font.
    GlyphList font_atlas_;
    /// The texture ID for OpenGL to use when rendering.
    GLuint texture_id_;
    /// The next available texture layer to use when loading a new glyph.
    GLuint texture_position_;
    /// The size of the atlas texture. This includes the maximum ascender and descender.
    glm::uvec2 texture_size_;
    /// The maximum number of layers allowed in the glyph atlas.
    GLint texture_max_layers_;

    /// Loads a glyph from the font, and stores it in the atlas texture.
    /** 
     * @param[in] character The character to load.
     * @returns The texture layer where the loaded glyph has been stored.
     */
    GLint LoadChar(const FT_ULong& character);
  };
}

#endif // ! FONT_ATLAS_H
