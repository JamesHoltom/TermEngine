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

  /// Stores a list of font atlas's.
  extern FontAtlasList font_atlas_list;

  /// Checks if the given font path/size exists in the list.
  /**
   * @param[in] key The font path/size pair to check.
   * @returns If the pair exists in the list.
   */
  bool FontAtlasExists(const FontAtlasKey& key);

  /// Returns a font atlas from the list.
  /** 
   * @param[in] key The font path/size pair to retrieve.
   * @returns A pointer to the font atlas, or `nullptr` if it was not found.
   */
  FontAtlasPtr GetFontAtlas(const FontAtlasKey& key);
  
  /// Adds a new font atlas to the list.
  /**
   * @param[in] key The font path/size pair to add.
   * @returns A pointer to the newly created font atlas.
   */
  FontAtlasPtr AddFontAtlas(const FontAtlasKey& key);

  /// Removes a font atlas from the list.
  /** 
   * @param[in] key The font path/size pair to remove.
   */
  void RemoveFontAtlas(const FontAtlasKey& key);

  /// Removes all font atlas from the list.
  void CleanUpFontAtlas();

  /// Prints the number of references to all font atlas in the list.
  /**
   * Each count also includes the list's reference.
   */
  void GetPointerUsage();
}

#endif // ! FONT_ATLAS_MANAGER_H
