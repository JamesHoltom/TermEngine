/// @author James Holtom

#ifndef BACKGROUND_BINDINGS_H
#define BACKGROUND_BINDINGS_H

#include "../../background/Background.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the background-related script functions to the Lua state.
  /**
   * @params[in] state The lua state to bind to.
   */
  void BindBackgroundToState(sol::state& state)
  {
    state.create_named_table("background",
      "set", &background::SetBackground,
      "remove", &background::RemoveBackground);
  }
}

#endif // ! BACKGROUND_BINDINGS_H
