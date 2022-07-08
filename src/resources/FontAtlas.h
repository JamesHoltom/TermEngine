/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/glmUtils.h"

namespace term_engine::fonts {
  struct GlyphBB;

  /// Used to store characters and the texture layers of the glyph they represent.
  typedef std::unordered_map<FT_UInt32, GlyphBB> GlyphList;

  /// Defines the bounding position, size and baseline of a glyph within the font atlas.
  struct GlyphBB {
    glm::ivec2 position_;
    glm::ivec2 size_;
    int baseline_;
  };

  /// The default font path to use when running the engine.
#ifdef linux
  constexpr char DEFAULT_FONT[] = "truetype/ubuntu/UbuntuMono-R.ttf";
  //constexpr char DEFAULT_FONT[] = "/home/jholtom/Programming/TermEngine/build/projects/example/07c_fonts/resources/fonts/SpaceMono-Regular.ttf";
#elif defined(_WIN32) || defined (WIN32)
  constexpr char DEFAULT_FONT[] = "arial.ttf";
#endif
  /// The default font size to use when running the engine.
  constexpr int DEFAULT_FONT_SIZE = 32;
  /// Should the default font be square (i.e. the width/height are equal)?
  constexpr bool IS_FONT_SQUARE = true;
  /// The size of the texture to store font characters in.
  constexpr GLsizei TEXTURE_SIZE = 1024;
  /// Defines an empty glyph that is returned when one fails to load, or a zero-character (i.e. '\0') is loaded.
  constexpr GlyphBB EMPTY_GLYPH = { glm::ivec2(), glm::ivec2() };
  /// The default number of spaces that make up a tab character (i.e. '\t').
  constexpr unsigned int DEFAULT_TAB_SIZE = 2;
  
  /// The relative filepath of the font being used.
  extern std::string font_path;
  /// The font size, in pixels (px).
  extern int font_size;
  /// The glyph size, in pixels (px);
  extern glm::ivec2 glyph_size;
  /// A handler for the loaded font face. This also refers to the currently loaded glyph.
  extern FT_Face font_face;
  /// The list containing all glyphs loaded from the font.
  extern GlyphList font_atlas;
  /// The texture ID for OpenGL to use when rendering.
  extern GLuint texture_id;
  /// The amount of glyphs currently stored in the font atlas.
  extern GLuint glyph_count;
  /// The position to store the next loaded glyph at.
  extern glm::uvec2 next_pos;
  /// The tallest height value of a glyph in the current row. This is used to cleanly move to the next row, after filling the current one.
  extern GLuint max_height;
  /// The amount of spaces to render when rendering a tab character (i.e. '\t').
  extern unsigned int tab_size;

  /// Initialises the font atlas and prepares it for use.
  /**
   * @returns If the font atlas was successfully set up.
   */
  bool Init();

  /// Destroys the font atlas.
  void CleanUp();

  /// Finds a glyph in the atlas and returns the texture layer the glyph is on.
  /**
    * If no glyph is found, it is loaded from the font and stored in the atlas.
    *
    * @param[in] character The character to look up.
    * @returns The texture layer that holds the character.
    */
  GlyphBB GetCharacter(const FT_ULong& character);

  /// Creates the texture of a character, and stores it in the atlas texture.
  /**
   * @param[in] character The character to render.
   * @returns The bounding box of the loaded glyph.
   */
  std::pair<bool, GlyphBB> _CreateCharTexture(const FT_ULong& character);

  /// Loads a glyph from the font.
  /**
   * @param[in] character The character to load.
   * @returns The bounding box of the loaded glyph.
   */
  GlyphBB _LoadChar(const FT_ULong& character);

  /// Sets the font being used to render characters.
  /**
   * All existing characters in the font atlas are re-rendered when this is called.
   * @param[in] filename The path to the font file to load.
   * @param[in] size     The size, in pixels (px), to render the characters at.
   * @param[in] isSquare Should the glyph size be square (i.e. the width and height are equal)?
   * @returns If the font was successfully loaded.
   */
  bool SetFont(const std::string& filename, const FT_Int& size, const bool& isSquare);

  /// Unloads the font currently in use. This is used before loading a new one.
  void _RemoveFont();

  /// Clears the font atlas texture.
  void _ClearCache();

  /// Resets the glyph size, according to the current font.
  /**
   * @param[in] isSquare Should the glyph size be square (i.e. the width and height are equal)?
   */
  void ResetGlyphSize(const bool& isSquare);

  /// Gets the path to the font currently in use.
  /**
   * @returns The path to the font.
   */
  std::string GetFontPath();

  /// Gets the path to the default font.
  /**
   * @returns The path to the font.
   */
  std::string GetDefaultFontPath();

  /// Gets the font size, in pixels (px).
  /**
   * @returns The font size.
   */
  int GetFontSize();

  /// Gets the default font size, in pixels (px).
  /**
   * @returns The font size.
   */
  int GetDefaultFontSize();

  /// Gets the size of a glyph, in pixels (px).
  /**
   * @returns The glyph size.
   */
  glm::ivec2 GetGlyphSize();
}

#endif // ! FONT_ATLAS_H
