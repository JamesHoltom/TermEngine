#include "EventManager.h"
#include "../utility/DebugFunctions.h"

namespace term_engine::events {
  void DoEvents(sol::state& state)
  {
    SDL_Event event;
    std::string key;

    while (SDL_PollEvent(&event)) {
      debug::LogKeyboardEvents(event);
      debug::LogWindowEvents(event);

      switch (event.type) {
      case SDL_KEYDOWN:
        key = SDL_GetScancodeName(event.key.keysym.scancode);

        state["OnKeyDown"](key);

        if (events::ScancodeIsPressed(event.key.keysym.scancode)) {
          state["OnKeyPress"](key);
        }

        break;
      case SDL_KEYUP:
        key = SDL_GetScancodeName(event.key.keysym.scancode);

        if (events::ScancodeIsReleased(event.key.keysym.scancode)) {
          state["OnKeyRelease"](key);
        }

        break;
      case SDL_MOUSEBUTTONDOWN:
        state["OnMouseDown"](event.button.button);

        if (events::MouseIsPressed(event.button.button)) {
          state["OnMousePress"](event.button.button);
        }

        break;
      case SDL_MOUSEBUTTONUP:
        if (events::MouseIsReleased(event.button.button)) {
          state["OnMouseRelease"](event.button.button);
        }

        break;
      case SDL_MOUSEMOTION:
        state["OnMouseMove"]();

        break;
      }
    }
  }
}
