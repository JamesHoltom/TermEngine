/// @author James Holtom

#ifndef PROJECT_BINDINGS_H
#define PROJECT_BINDINGS_H

#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the project-related script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindProjectToState(sol::state& state)
  {
    /* state.create_named_table("project",
      "load", [&](const std::string& project_path) {},
      "reload", [&]() {},
      "unload", [&]() {}); */
  }
}

#endif // ! PROJECT_BINDINGS_H
