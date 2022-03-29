/// @author James Holtom

#ifndef EVENT_BINDINGS_H
#define EVENT_BINDINGS_H

#include "../../events/InputManager.h"
#include "../../logging/Logger.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds the input and event-related script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
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
      "position", &events::GetMousePosition,
      "movement", &events::GetMouseMovement);

    state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);

    // Create bindings for the main game functions.
    state["Init"] = [&]() -> bool {
      logging::logger->info("TermEngine has initialised!");

      return true;
    };
    state["Loop"] = [&](const float& timestep) -> void {};
    state["Quit"] = [&]() -> bool {
      logging::logger->info("TermEngine is quitting!");

      return true;
    };

    state["OnMouseMove"] = [&](const glm::ivec2& delta) -> void {};
    state["OnMouseDown"] = [&](const int& button) -> void {};
    state["OnMousePress"] = [&](const int& button) -> void {};
    state["OnMouseRelease"] = [&](const int& button) -> void {};

    state["OnKeyDown"] = [&](const std::string& key) -> void {};
    state["OnKeyPress"] = [&](const std::string& key) -> void {};
    state["OnKeyRelease"] = [&](const std::string& key) -> void {};
  }
}

#endif // ! EVENT_BINDINGS_H
