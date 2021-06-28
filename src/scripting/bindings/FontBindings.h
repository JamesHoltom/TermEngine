/// @author James Holtom

#ifndef FONT_BINDINGS_H
#define FONT_BINDINGS_H

#include "../../fonts/FontAtlas.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the font-related script functions to the Lua state.
  /**
   * @params[in] state The lua state to bind to.
   */
  void BindFontToState(sol::state& state)
  {
    state.create_named_table("font",
      "set", &fonts::SetFont,
      "fontName", [&]() -> std::string { return fonts::font_path; },
      "fontSize", [&]() -> int { return fonts::font_size; },
      "defaultFontName", [&]() -> std::string { return std::string(fonts::DEFAULT_FONT); },
      "defaultFontSize", [&]() -> int { return fonts::DEFAULT_FONT_SIZE; });
  }
}

#endif // ! FONT_BINDINGS_H
