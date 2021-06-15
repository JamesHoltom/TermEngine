/// @author James Holtom

#ifndef FONT_BINDINGS_H
#define FONT_BINDINGS_H

#include "../../fonts/FontAtlas.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindFontToState(sol::state& state)
  {
    state.create_named_table("font",
      "set", &fonts::SetFont,
      "getName", &fonts::font_path,
      "getSize", &fonts::font_size);
  }
}

#endif // ! FONT_BINDINGS_H
