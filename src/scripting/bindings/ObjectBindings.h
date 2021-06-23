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
      sol::meta_function::construct, sol::factories(&objects::Add),
      sol::call_constructor, sol::factories(&objects::Add),
      sol::meta_function::garbage_collect, sol::destructor(&objects::Remove),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::property(&objects::Object::GetData));

    state.create_named_table("objects",
      "count", [&]() -> size_t { return objects::object_list.size(); },
      "dirty", [&]() { objects::Object::SetDirty(true); },
      "is_dirty", &objects::Object::IsDirty);
  }
}

#endif // ! OBJECT_BINDINGS_H
