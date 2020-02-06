#include "Glyph.h"

namespace term_engine::utilities {
  Glyph nullGlyph = {
    L'\u0000',
    { 255, 255, 255, 255},
    { 0, 0, 0, 255}
  };
}