/// @author James Holtom

#ifndef FT_UTILS_H
#define FT_UTILS_H

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace FT {
  /// Logs any errors that occur when calling a FreeType function.
  /**
   * @param[in] result The result of the FreeType call to check.
   * @returns The result that was passed in.
   */
  FT_Error Log(FT_Error result);
}

#endif // ! FT_UTILS_H

