/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::fonts {
  /// Used to store characters and the texture layers of the glyph they represent.
  typedef std::unordered_map<FT_UInt32, GLuint> GlyphList;

  /// The default font path to use when running the engine.
  constexpr char DEFAULT_FONT[] = "resources/fonts/RobotoMono-Regular.ttf";
  /// The default font size to use when running the engine.
  constexpr int DEFAULT_FONT_SIZE = 16;
  
  /// The relative filepath of the font being used.
  extern std::string font_path;
  /// The font size, in pixels (px).
  extern FT_UInt font_size;
  /// A handler for the loaded font face. This also refers to the currently loaded glyph.
  extern FT_Face font_face;
  /// The list containing all glyphs loaded from the font.
  extern GlyphList font_atlas;
  /// The texture ID for OpenGL to use when rendering.
  extern GLuint texture_id;
  /// The amount of glyphs currently stored in the font atlas.
  extern GLuint glyph_count;
  /// The size of the atlas texture. This includes the maximum ascender and descender.
  extern glm::vec2 texture_size;
  /// The maximum amount of layers in the atlas texture array.
  extern GLint texture_max_layers;

  /// Initialises the font atlas and prepares it for use.
  /**
   * @returns If the font atlas was successfully set up.
   */
  int Init();

  /// Destroys the font atlas.
  void CleanUp();

  /// Finds a glyph in the atlas and returns the texture layer the glyph is on.
  /**
    * If no glyph is found, it is loaded from the font and stored in the atlas.
    *
    * @param[in] character The character to look up.
    * @returns The texture layer that holds the character.
    */
  GLint GetCharacter(const FT_ULong& character);

  /// Loads a glyph from the font, and stores it in the atlas texture.
  /**
    * @param[in] character The character to load.
    * @returns The texture layer where the loaded glyph has been stored.
    */
  GLint _LoadChar(const FT_ULong& character);

  /// Creates the texture of a character, and stores it in the font texture.
  /**
   * @param[in] character     The character to render.
   * @param[in] texture_layer The texture layer to render the character to.
   * @returns If the character was successfully rendered.
   */
  bool _CreateCharTexture(const FT_ULong& character, const GLint texture_layer);

  /// Sets the font being used to render characters.
  /**
   * All existing characters in the font atlas are re-rendered when this is called.
   * @param[in] filename The path to the font file to load.
   * @param[in] size     The size, in pixels (px), to render the characters at.
   * @returns If the font was successfully loaded.
   */
  int SetFont(const std::string& filename, const FT_UInt& size);
}

#endif // ! FONT_ATLAS_H
