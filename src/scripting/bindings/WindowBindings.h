/// @author James Holtom

#ifndef WINDOW_BINDINGS_H
#define WINDOW_BINDINGS_H

#include "../../system/Window.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindWindowToState(sol::state& state)
  {
    state.create_named_table("window",
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "getClearColor", &system::GetWindowClearColor,
      "setClearColor", &system::SetWindowClearColor);

    state.create_named_table("wireframe",
      "enable", &system::EnableWireframe,
      "disable", &system::DisableWireframe);
  }
}

#endif // ! WINDOW_BINDINGS_H
