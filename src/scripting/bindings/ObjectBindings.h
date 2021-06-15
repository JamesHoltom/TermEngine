/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include "../../objects/ObjectManager.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindObjectToState(sol::state& state)
  {
    state.new_usertype<objects::Object>(
      "Object",
      sol::no_constructor,
      "name", sol::readonly_property(&objects::Object::GetName),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::readonly_property(&objects::Object::GetData));

    state.create_named_table("objects",
      "add", &objects::Add,
      "get", &objects::Get,
      "remove", &objects::Remove,
      "dirty", [&]() { objects::is_dirty = true; },
      "is_dirty", [&]() { return objects::is_dirty; });
  }
}

#endif // ! OBJECT_BINDINGS_H
