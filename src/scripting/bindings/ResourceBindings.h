/// @author James Holtom

#ifndef RESOURCE_BINDINGS_H
#define RESOURCE_BINDINGS_H

#include "../../resources/Background.h"
#include "../../resources/FontAtlas.h"
#include "../../resources/Glyph.h"
#include "../../resources/Window.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the resource-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindResourcesToState(sol::state& state)
  {
    state.create_named_table(
      "background",
      "set", &background::SetBackground,
      "remove", &background::RemoveBackground);
    
    state.create_named_table(
      "font",
      "set", &fonts::SetFont,
      "isSquare", &fonts::ResetGlyphSize,
      "path", &fonts::GetFontPath,
      "size", &fonts::GetFontSize,
      "defaultPath", &fonts::GetDefaultFontPath,
      "defaultSize", &fonts::GetDefaultFontSize,
      "tabSize", fonts::tab_size);

    state.new_usertype<resources::GlyphParams>(
        "Glyph",
        sol::meta_function::construct, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
        sol::call_constructor, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
        sol::meta_function::equal_to, sol::overload([](const resources::GlyphParams& lhs, const resources::GlyphParams& rhs) { return lhs == rhs; }),
        "character", &resources::GlyphParams::character_,
        "foreground_colour", &resources::GlyphParams::foreground_colour_,
        "background_colour", &resources::GlyphParams::background_colour_);

    state.create_named_table(
      "glyphs",
      "size", &fonts::GetGlyphSize,
      "NO_CHARACTER", resources::NO_CHARACTER,
      "DEFAULT_FOREGROUND_COLOUR", resources::DEFAULT_FOREGROUND_COLOUR,
      "DEFAULT_BACKGROUND_COLOUR", resources::DEFAULT_BACKGROUND_COLOUR);

    state.create_named_table(
      "window",
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "fitToView", &system::ResizeWindowToView,
      "getClearColour", &system::GetWindowClearColour,
      "setClearColour", &system::SetWindowClearColour);
  }
}

#endif // ! RESOURCE_BINDINGS_H
