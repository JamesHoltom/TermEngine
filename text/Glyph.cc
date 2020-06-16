#include "Glyph.h"

namespace term_engine {
  Glyph nullGlyph = {
    L'\u0000',
    { 255, 255, 255, 255},
    { 0, 0, 0, 255}
  };
}