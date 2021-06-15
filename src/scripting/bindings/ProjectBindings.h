/// @author James Holtom

#ifndef PROJECT_BINDINGS_H
#define PROJECT_BINDINGS_H

#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindProjectToState(sol::state& state)
  {
    /* state.create_named_table("project",
      "load", [&](const std::string& project_path) {},
      "reload", [&]() {},
      "unload", [&]() {}); */
  }
}

#endif // ! PROJECT_BINDINGS_H
