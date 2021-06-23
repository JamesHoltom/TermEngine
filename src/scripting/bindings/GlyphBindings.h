/// @author James Holtom

#ifndef GLYPH_BINDINGS_H
#define GLYPH_BINDINGS_H

#include "../../glyphs/Glyph.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindGlyphToState(sol::state& state)
  {
    state.new_usertype<glyphs::GlyphParams>(
      "Glyph",
      sol::meta_function::construct, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
      sol::call_constructor, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
      sol::meta_function::equal_to, sol::overload([](const glyphs::GlyphParams& lhs, const glyphs::GlyphParams& rhs) { return lhs == rhs; }),
      "character", &glyphs::GlyphParams::character_,
      "foreground_color", &glyphs::GlyphParams::foreground_color_,
      "background_color", &glyphs::GlyphParams::background_color_);

    state.create_named_table("glyphs",
      "NO_CHARACTER", glyphs::NO_CHARACTER,
      "DEFAULT_FG", glyphs::DEFAULT_FOREGROUND_COLOR,
      "DEFAULT_BG", glyphs::DEFAULT_BACKGROUND_COLOR);
  }
}

#endif // ! GLYPH_BINDINGS_H
