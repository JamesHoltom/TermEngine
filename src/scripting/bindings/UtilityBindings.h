/// @author James Holtom

#ifndef UTILITY_BINDINGS_H
#define UTILITY_BINDINGS_H

#include "../../resources/Window.h"
#include "../../utility/IndexUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds any utility script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindUtilitiesToState(sol::state& state)
  {
    state.create_named_table("wireframe",
      "enable", &system::EnableWireframe,
      "disable", &system::DisableWireframe);

    state.set_function("round", lroundf);
    
    state.set_function("getIndexFromPosition", GetIndexFromPosition);
    state.set_function("getRowColFromPosition", GetRowColFromPosition);
    state.set_function("getPositionFromIndex", GetPositionFromIndex);
    state.set_function("getRowColFromIndex", GetRowColFromIndex);
    state.set_function("getIndexFromRowCol", GetIndexFromRowCol);
    state.set_function("getPositionFromRowCol", GetPositionFromRowCol);
  }
}

#endif // ! UTILITY_BINDINGS_H
