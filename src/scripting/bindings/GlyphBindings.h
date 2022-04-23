/// @author James Holtom

#ifndef GLYPH_BINDINGS_H
#define GLYPH_BINDINGS_H

#include "../../utility/Glyph.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the _Glyph_ type as a usertype to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindGlyphToState(sol::state& state)
  {
    state.new_usertype<GlyphParams>(
      "Glyph",
      sol::meta_function::construct, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
      sol::call_constructor, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
      sol::meta_function::equal_to, sol::overload([](const GlyphParams& lhs, const GlyphParams& rhs) { return lhs == rhs; }),
      "character", &GlyphParams::character_,
      "foreground_color", &GlyphParams::foreground_color_,
      "background_color", &GlyphParams::background_color_);

    state.create_named_table("glyphs",
      "NO_CHARACTER", NO_CHARACTER,
      "DEFAULT_FG", DEFAULT_FOREGROUND_COLOR,
      "DEFAULT_BG", DEFAULT_BACKGROUND_COLOR);
  }
}

#endif // ! GLYPH_BINDINGS_H
