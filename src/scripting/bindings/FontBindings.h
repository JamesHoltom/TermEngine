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
      "font_name", [&]() { return fonts::font_path; },
      "font_size", [&]() { return fonts::font_size; },
      "DEFAULT_FONT_NAME", std::string(fonts::DEFAULT_FONT),
      "DEFAULT_FONT_SIZE", fonts::DEFAULT_FONT_SIZE);
  }
}

#endif // ! FONT_BINDINGS_H
