/// @author James Holtom

#ifndef EVENT_BINDINGS_H
#define EVENT_BINDINGS_H

#include "../../events/InputManager.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the input and event-related script functions to the Lua state.
  /**
   * @params[in] state The lua state to bind to.
   */
  void BindEventToState(sol::state& state)
  {
    state.create_named_table("mouse",
      "LEFT", sol::var(SDL_BUTTON_LEFT),
      "MIDDLE", sol::var(SDL_BUTTON_MIDDLE),
      "RIGHT", sol::var(SDL_BUTTON_RIGHT),
      "isDown", &events::MouseIsDown,
      "isPressed", &events::MouseIsPressed,
      "isReleased", &events::MouseIsReleased,
      "position", [&]() -> glm::ivec2 { return events::mouse_position; },
      "movement", [&]() -> glm::ivec2 { return events::mouse_position_delta; });

    state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);
  }
}

#endif // ! EVENT_BINDINGS_H
