/// @author James Holtom

#ifndef OBJECT_BINDINGS_H
#define OBJECT_BINDINGS_H

#include "../../objects/ObjectManager.h"
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
      sol::meta_function::construct, sol::factories(&objects::Add),
      sol::call_constructor, sol::factories(&objects::Add),
      sol::meta_function::garbage_collect, sol::destructor(&objects::Remove),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::property(&objects::Object::GetData),
      "active", sol::property(&objects::Object::IsActive, &objects::Object::SetActive),
      "release", &objects::Remove);

    state.create_named_table("objects",
      "count", &objects::Count,
      "is_dirty", &objects::Object::IsDirty,
      "dirty", &objects::Object::SetDirty);
  }
}

#endif // ! OBJECT_BINDINGS_H
