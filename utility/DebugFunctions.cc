#include "DebugFunctions.h"

namespace term_engine::debug {
  void LogKeyboardEvents(SDL_Event& event) {
    std::string keyboard_type;
    std::string keyboard_modifiers;

    switch (event.type) {
    case SDL_KEYDOWN:
      keyboard_type = "Pressed";

      break;
    case SDL_KEYUP:
      keyboard_type = "Released";

      break;
    default:
      return;
    }

    if (event.key.keysym.mod == KMOD_NONE) {
      keyboard_modifiers = "None";
    }
    else {
      if (event.key.keysym.mod & KMOD_ALT) {
        keyboard_modifiers += ", Alt";
      }

      if (event.key.keysym.mod & KMOD_CTRL) {
        keyboard_modifiers += ", Ctrl";
      }

      if (event.key.keysym.mod & KMOD_SHIFT) {
        keyboard_modifiers += ", Shift";
      }

      if (event.key.keysym.mod & KMOD_GUI) {
        keyboard_modifiers += ", Windows";
      }

      if (event.key.keysym.mod & KMOD_CAPS) {
        keyboard_modifiers += ", Caps Lock";
      }

      if (event.key.keysym.mod & KMOD_NUM) {
        keyboard_modifiers += ", Num Lock";
      }

      if (event.key.keysym.mod & KMOD_MODE) {
        keyboard_modifiers += ", AltGr";
      }

      keyboard_modifiers.erase(0, 2);
    }

    spdlog::info("Keyboard Event: {}, Key: {}, Modifiers: {}", keyboard_type, SDL_GetKeyName(event.key.keysym.sym), keyboard_modifiers);
  }

  void LogWindowEvents(SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT) {
      std::string window_string = "";
      bool showXY = false;

      switch (event.window.event) {
      case SDL_WINDOWEVENT_CLOSE:        window_string = "Closed";                      break;
      case SDL_WINDOWEVENT_ENTER:        window_string = "Mouse Entered";               break;
      case SDL_WINDOWEVENT_LEAVE:        window_string = "Mouse Left";                  break;
      case SDL_WINDOWEVENT_FOCUS_GAINED: window_string = "Focus Gained";                break;
      case SDL_WINDOWEVENT_FOCUS_LOST:   window_string = "Focus Lost";                  break;
      case SDL_WINDOWEVENT_EXPOSED:      window_string = "Exposed";                     break;
      case SDL_WINDOWEVENT_HIDDEN:       window_string = "Hidden";                      break;
      case SDL_WINDOWEVENT_SHOWN:        window_string = "Shown";                       break;
      case SDL_WINDOWEVENT_MAXIMIZED:    window_string = "Maximised";                   break;
      case SDL_WINDOWEVENT_MINIMIZED:    window_string = "Minimised";                   break;
      case SDL_WINDOWEVENT_RESTORED:     window_string = "Restored";                    break;
      case SDL_WINDOWEVENT_MOVED:        window_string = "Moved";        showXY = true; break;
      case SDL_WINDOWEVENT_RESIZED:      window_string = "Resized";      showXY = true; break;
      case SDL_WINDOWEVENT_SIZE_CHANGED: window_string = "Size Changed"; showXY = true; break;
      default:                           window_string = "Unknown";                     break;
      }

      if (window_string != "") {
        if (showXY) {
          spdlog::info("Window ID: {}, Event: {}, X: {}, Y: {}", event.window.windowID, window_string, event.window.data1, event.window.data2);
        }
        else {
          spdlog::info("Window ID: {}, Event: {}", event.window.windowID, window_string);
        }
      }
    }
  }
}
