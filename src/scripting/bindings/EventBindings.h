/// @author James Holtom

#ifndef EVENT_BINDINGS_H
#define EVENT_BINDINGS_H

#include "../../events/InputManager.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindEventToState(sol::state& state)
  {
    state.create_named_table("mouse",
      "LEFT", sol::var(SDL_BUTTON_LEFT),
      "MIDDLE", sol::var(SDL_BUTTON_MIDDLE),
      "RIGHT", sol::var(SDL_BUTTON_RIGHT),
      "isDown", &events::MouseIsDown,
      "isPressed", &events::MouseIsPressed,
      "isReleased", &events::MouseIsReleased,
      "position", sol::var(&events::mouse_position),
      "movement", sol::var(&events::mouse_position_delta));

    state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);
  }
}

#endif // ! EVENT_BINDINGS_H
