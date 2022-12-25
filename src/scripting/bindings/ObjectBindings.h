/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include <glm/glm.hpp>
#include "../../objects/Character.h"
#include "../../objects/Object.h"
#include "../../objects/ShaderProgram.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the _Object_ type as a usertype, as well as other object-related script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindObjectToState(sol::state& state)
  {
    state.new_usertype<objects::Object>(
      "Object",
      sol::meta_function::construct, sol::factories(&objects::Object::Add, &objects::Object::AddSelf),
      sol::call_constructor, sol::factories(&objects::Object::Add),
      sol::meta_function::garbage_collect, sol::destructor(&objects::Object::Remove),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::property(&objects::Object::GetData),
      "set", &objects::Object::Set,
      "active", sol::property(&objects::Object::IsActive, &objects::Object::SetActive),
      "release", &objects::Object::Remove);

    state.create_named_table("objects",
      "getCount", &objects::Object::Count,
      "isDirty", &objects::Object::IsDirty,
      "dirty", &objects::Object::Dirty);
    
    state.new_usertype<objects::CharacterParams>(
        "Character",
        sol::meta_function::construct, sol::constructors<void(),
                                                         void(const objects::CharacterParams&),
                                                         void(const char&, const glm::vec4&, const glm::vec4&)>(),
        sol::call_constructor, sol::constructors<void(),
                                                 void(const objects::CharacterParams&),
                                                 void(const char&, const glm::vec4&, const glm::vec4&)>(),
        sol::meta_function::equal_to, sol::overload([](const objects::CharacterParams& lhs, const objects::CharacterParams& rhs) { return lhs == rhs; }),
        "character", &objects::CharacterParams::character_,
        "foreground_colour", &objects::CharacterParams::foreground_colour_,
        "background_colour", &objects::CharacterParams::background_colour_);

    state.create_named_table(
      "characters",
      "NO_CHARACTER", objects::NO_CHARACTER,
      "DEFAULT_FOREGROUND_COLOUR", objects::DEFAULT_FOREGROUND_COLOUR,
      "DEFAULT_BACKGROUND_COLOUR", objects::DEFAULT_BACKGROUND_COLOUR);

    state.new_usertype<objects::ShaderProgram>(
      "Shader",
      sol::meta_function::construct, sol::factories(&objects::ShaderProgram::Add, &objects::ShaderProgram::AddSelf),
      sol::call_constructor, sol::factories(&objects::ShaderProgram::Add),
      sol::meta_function::garbage_collect, sol::destructor(&objects::ShaderProgram::Remove),
      "attach", &objects::ShaderProgram::AttachFile,
      "link", &objects::ShaderProgram::Link,
      "linked", sol::readonly_property(&objects::ShaderProgram::GetProgramId),
      "use", &objects::ShaderProgram::Use
    );

    state.create_named_table("shaders",
      "getCount", &objects::Object::Count);
  }
}

#endif // ! OBJECT_BINDINGS_H
