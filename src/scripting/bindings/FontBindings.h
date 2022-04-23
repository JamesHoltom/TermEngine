/// @author James Holtom

#ifndef FONT_BINDINGS_H
#define FONT_BINDINGS_H

#include "../../resources/FontAtlas.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the font-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindFontToState(sol::state& state)
  {
    state.create_named_table("font",
      "set", &fonts::SetFont,
      "path", &fonts::GetFontPath,
      "size", &fonts::GetFontSize,
      "textureSize", &fonts::GetTextureSize,
      "defaultPath", &fonts::GetDefaultFontPath,
      "defaultSize", &fonts::GetDefaultFontSize);
  }
}

#endif // ! FONT_BINDINGS_H
