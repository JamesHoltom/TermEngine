/// @author James Holtom

#ifndef CORE_BINDINGS_H
#define CORE_BINDINGS_H

#include "../../utility/SolUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the input and event-related script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindCoreToState(sol::state& state)
  {
    // Create bindings for the main game functions.
    state["Init"] = [&]() -> bool {
      utility::logger->info("TermEngine has initialised!");

      return true;
    };
    state["Loop"] = [&](uint64_t timestep) -> void {};
    state["Quit"] = [&]() -> void {
      utility::logger->info("TermEngine is quitting!");
    };
  }
}

#endif // ! CORE_BINDINGS_H
