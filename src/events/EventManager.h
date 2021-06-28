/// @author James Holtom

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "../events/InputManager.h"
#include "../utility/SDLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::events {
  /// Does event checking, and sends events to Lua if any are found.
  /**
   * @param[in] state The Lua state to send events to.
   */
  void DoEvents(sol::state& state);
}

#endif // ! EVENT_MANAGER_H
