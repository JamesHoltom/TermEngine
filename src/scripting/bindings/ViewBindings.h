/// @author James Holtom

#ifndef VIEW_BINDINGS_H
#define VIEW_BINDINGS_H

#include "../../utility/SolUtils.h"
#include "../../view/View.h"

namespace term_engine::scripting::bindings {
  /// Binds the view-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindViewToState(sol::state& state)
  {
    state.create_named_table("view",
      "getPosition", &views::GetPosition,
      "setPosition", &views::SetPosition,
      "getSize", &views::GetSize,
      "setSize", &views::SetSize);
  }
}

#endif // ! VIEW_BINDINGS_H
