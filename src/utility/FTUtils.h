/// @author James Holtom

#ifndef FT_UTILS_H
#define FT_UTILS_H

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace term_engine::utility {
  /// @brief The FreeType library instance used to manage and render fonts.
  extern FT_Library font_library;

  /**
   * @brief Logs any errors that occur when calling a FreeType function.
   * 
   * @param[in] result The result of the FreeType call to check.
   * @returns The result that was passed in.
   */
  FT_Error FTLog(FT_Error result);

  /**
   * @brief Initialises FreeType.
   * 
   * @param[in,out] library An instance of the FreeType library to initialise.
   * @returns Was FreeType successfully initialised?
   */
  int InitFreeType();

  /**
   * @brief Shuts down FreeType.
   * 
   * @param[in,out] library An instance of the FreeType library to shut down.
   */
  void CleanUpFreeType();
}

#endif // ! FT_UTILS_H

