/// @author James Holtom

#ifndef WINDOW_BINDINGS_H
#define WINDOW_BINDINGS_H

#include "../../system/Window.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the window-related script functions to the Lua state.
  /**
   * @params[in] state The lua state to bind to.
   */
  void BindWindowToState(sol::state& state)
  {
    state.create_named_table("window",
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "fitToView", &system::ResizeWindowToView,
      "getClearColor", &system::GetWindowClearColor,
      "setClearColor", &system::SetWindowClearColor);
  }
}

#endif // ! WINDOW_BINDINGS_H
