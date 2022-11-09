/// @author James Holtom

#ifndef EVENT_BINDINGS_H
#define EVENT_BINDINGS_H

#include "../../events/InputManager.h"
#include "../../events/Listener.h"
#include "../../logging/Logger.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the input and event-related script functions to the Lua state.
   * 
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
      "getPosition", &events::GetMousePosition,
      "getMovement", &events::GetMouseMovement);

    state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);

    state.new_usertype<events::EventListener>(
      "EventListener",
      sol::meta_function::construct, sol::factories(&events::EventListener::Add),
      sol::call_constructor, sol::factories(&events::EventListener::Add),
      sol::meta_function::garbage_collect, sol::destructor(&events::EventListener::Remove),
      "active", sol::property(&events::EventListener::IsActive, &events::EventListener::SetActive),
      "type", sol::property(&events::EventListener::GetType),
      "trigger", &events::EventListener::Trigger,
      "release", &events::EventListener::Remove);

    state.create_named_table("event",
      "countListeners", &events::EventListener::Count,
      "clearListeners", &events::EventListener::Clear,
      "getListenerTypes", &events::EventListener::GetTypes);

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
  }
}

#endif // ! EVENT_BINDINGS_H
