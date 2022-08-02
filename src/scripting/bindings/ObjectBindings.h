/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include <glm/glm.hpp>
#include "../../objects/Glyph.h"
#include "../../objects/Object.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the _Object_ type as a usertype, as well as other object-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindObjectToState(sol::state& state)
  {
    state.new_usertype<objects::Object>(
      "Object",
      sol::meta_function::construct, sol::factories(&objects::Object::Add),
      sol::call_constructor, sol::factories(&objects::Object::Add),
      sol::meta_function::garbage_collect, sol::destructor(&objects::Object::Remove),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::property(&objects::Object::GetData),
      "active", sol::property(&objects::Object::IsActive, &objects::Object::SetActive),
      "release", &objects::Object::Remove);

    state.create_named_table("objects",
      "count", &objects::Object::Count,
      "is_dirty", &objects::Object::IsDirty,
      "dirty", &objects::Object::SetDirty);
    
    state.new_usertype<objects::GlyphParams>(
        "Glyph",
        sol::meta_function::construct, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
        sol::call_constructor, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
        sol::meta_function::equal_to, sol::overload([](const objects::GlyphParams& lhs, const objects::GlyphParams& rhs) { return lhs == rhs; }),
        "character", &objects::GlyphParams::character_,
        "foreground_colour", &objects::GlyphParams::foreground_colour_,
        "background_colour", &objects::GlyphParams::background_colour_);

    state.create_named_table(
      "glyphs",
      "NO_CHARACTER", objects::NO_CHARACTER,
      "DEFAULT_FOREGROUND_COLOUR", objects::DEFAULT_FOREGROUND_COLOUR,
      "DEFAULT_BACKGROUND_COLOUR", objects::DEFAULT_BACKGROUND_COLOUR);

  }
}

#endif // ! OBJECT_BINDINGS_H
