/// @author James Holtom

#ifndef FONT_ATLAS_MANAGER_H
#define FONT_ATLAS_MANAGER_H

#include <string>

#include "FontAtlas.h"

namespace term_engine::fonts {
  /// The default font path to use when running the engine.
  constexpr char DEFAULT_FONT[] = "resources/fonts/lucon.ttf";
  /// The default font size to use when running the engine.
  constexpr int DEFAULT_FONT_SIZE = 16;

  /// The FreeType library instance used to manage and render fonts.
  extern FT_Library font_library;
  /// Stores a list of font atlas's.
  extern FontAtlasList font_atlas_list;

  /// Returns a font atlas from the list.
  /** 
   * @param[in] key_name The key name of the font atlas to get.
   * @returns A pointer to the font atlas, or `nullptr` if it was not found.
   */
  FontAtlasPtr GetFontAtlas(const std::string& key_name);
  
  /// Adds a new font atlas to the list.
  /** 
   * @param[in] font_path    The filepath to the font to initialise the font atlas with.
   * @param[in] glyph_height The font size to initialise the font atlas with.
   * @returns A pointer to the newly created font atlas.
   */
  FontAtlasPtr AddFontAtlas(const std::string& font_path, const int& glyph_height);

  /// Removes a font atlas from the list.
  /** 
   * @param[in] key_name The key name of the font atlas to remove.
   */
  void RemoveFontAtlas(const std::string& key_name);

  /// Removes all font atlas from the list.
  void CleanUpFontAtlas();

  /// Prints the number of references to all font atlas in the list.
  /**
   * Each count also includes the list's reference.
   */
  void GetPointerUsage();
}

#endif // ! FONT_ATLAS_MANAGER_H
