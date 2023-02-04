#include <glm/glm.hpp>
#include "Listener.h"
#include "../scripting/ScriptingInterface.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::events {
  EventQueue event_queue;
  
  void InitQueue()
  {
    event_queue.reserve(MAX_EVENTS);
  }

  void CleanUpQueue()
  {
    event_queue.clear();
  }

  void DoSDLEvents(const SDL_Event& event)
  {
    switch (event.type)
    {
      case SDL_KEYDOWN:
        event_queue.emplace_back("key_down", scripting::lua_state->create_table_with(
          "key", SDL_GetScancodeName(event.key.keysym.scancode),
          "alt", event.key.keysym.mod & KMOD_ALT,
          "ctrl", event.key.keysym.mod & KMOD_CTRL,
          "shift", event.key.keysym.mod & KMOD_SHIFT,
          "gui", event.key.keysym.mod & KMOD_GUI,
          "capslock", event.key.keysym.mod & KMOD_CAPS,
          "numlock", event.key.keysym.mod & KMOD_NUM,
          "scrolllock", event.key.keysym.mod & KMOD_SCROLL
        ));
        break;
      case SDL_KEYUP:
        event_queue.emplace_back("key_up", scripting::lua_state->create_table_with(
          "key", SDL_GetScancodeName(event.key.keysym.scancode),
          "alt", event.key.keysym.mod & KMOD_ALT,
          "ctrl", event.key.keysym.mod & KMOD_CTRL,
          "shift", event.key.keysym.mod & KMOD_SHIFT,
          "gui", event.key.keysym.mod & KMOD_GUI,
          "capslock", event.key.keysym.mod & KMOD_CAPS,
          "numlock", event.key.keysym.mod & KMOD_NUM,
          "scrolllock", event.key.keysym.mod & KMOD_SCROLL
        ));
        break;
      case SDL_MOUSEBUTTONDOWN:
        event_queue.emplace_back("mouse_down", scripting::lua_state->create_table_with(
          "button", event.button.button,
          "clicks", event.button.clicks,
          "position", glm::ivec2(event.button.x, event.button.y)
        ));
        break;
      case SDL_MOUSEBUTTONUP:
        event_queue.emplace_back("mouse_up", scripting::lua_state->create_table_with(
          "button", event.button.button,
          "clicks", event.button.clicks,
          "position", glm::ivec2(event.button.x, event.button.y)
        ));
        break;
      case SDL_MOUSEMOTION:
        event_queue.emplace_back("mouse_move", scripting::lua_state->create_table_with(
          "movement", glm::ivec2(event.motion.xrel, event.motion.yrel),
          "position", glm::ivec2(event.motion.x, event.motion.y)
        ));
        break;
      case SDL_MOUSEWHEEL:
        event_queue.emplace_back("mouse_scroll", scripting::lua_state->create_table_with(
          "movement", glm::vec2(event.wheel.preciseX, event.wheel.preciseY)
        ));
        break;
//      case SDL_TEXTINPUT:                 event_queue.emplace_back("text_input", scripting::lua_state->create_table());             break;
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_FOCUS_GAINED:  event_queue.emplace_back("window_keyboard_focus", scripting::lua_state->create_table());  break;
          case SDL_WINDOWEVENT_FOCUS_LOST:    event_queue.emplace_back("window_keyboard_blur", scripting::lua_state->create_table());   break;
          case SDL_WINDOWEVENT_ENTER:         event_queue.emplace_back("window_mouse_focus", scripting::lua_state->create_table());     break;
          case SDL_WINDOWEVENT_LEAVE:         event_queue.emplace_back("window_mouse_blur", scripting::lua_state->create_table());      break;
          case SDL_WINDOWEVENT_HIDDEN:        event_queue.emplace_back("window_hidden", scripting::lua_state->create_table());          break;
          case SDL_WINDOWEVENT_SHOWN:         event_queue.emplace_back("window_shown", scripting::lua_state->create_table());           break;
          case SDL_WINDOWEVENT_MINIMIZED:     event_queue.emplace_back("window_minimised", scripting::lua_state->create_table());       break;
          case SDL_WINDOWEVENT_MAXIMIZED:     event_queue.emplace_back("window_maximised", scripting::lua_state->create_table());       break;
          case SDL_WINDOWEVENT_RESTORED:      event_queue.emplace_back("window_restored", scripting::lua_state->create_table());        break;
          case SDL_WINDOWEVENT_MOVED:
            event_queue.emplace_back("window_moved", scripting::lua_state->create_table_with(
              "position", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
          case SDL_WINDOWEVENT_RESIZED:
            event_queue.emplace_back("window_resized", scripting::lua_state->create_table_with(
              "size", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
          case SDL_WINDOWEVENT_CLOSE:
            event_queue.emplace_back("window_close", scripting::lua_state->create_table());
            break;
      }
    }
  }
}