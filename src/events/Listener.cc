#include <glm/glm.hpp>
#include "Listener.h"
#include "../scripting/ScriptingInterface.h"
#include "../system/CLArguments.h"
#include "../utility/ConversionUtils.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/LogUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::events {
  void InitList()
  {
    event_list.reserve(MAX_EVENTS);
  }

  void CleanUpList()
  {
    event_list.clear();
  }

  void DoSDLEvents(const SDL_Event& event)
  {
    int window_id = 0;

    switch (event.type)
    {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        window_id = event.key.windowID;
        break;
      case SDL_TEXTINPUT:
        window_id = event.text.windowID;
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        window_id = event.button.windowID;
        break;
      case SDL_MOUSEMOTION:
        window_id = event.motion.windowID;
        break;
      case SDL_MOUSEWHEEL:
        window_id = event.wheel.windowID;
        break;
      case SDL_WINDOWEVENT:
        window_id = event.window.windowID;
        break;
      default:
        return;
    }

    if (utility::IsImguiWindow(window_id))
    {
      return;
    }

    usertypes::GameWindow* game_window = usertypes::GetGameWindowByWindowId(window_id);

    if (game_window == nullptr)
    {
      return;
    }

    bool captureKey = false;
    bool captureMouse = false;
    sol::table event_data = scripting::lua_state->create_table_with("game_window", game_window);
    std::string event_type = "";

    if (system::debug_mode)
    {
      ImGuiIO& io = ImGui::GetIO();

      captureKey = io.WantCaptureKeyboard;
      captureMouse = io.WantCaptureMouse;
    }

    if (!captureKey)
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          event_type = event.type == SDL_KEYDOWN ? "key_down" : "key_up";
          event_data.set(
            "key", SDL_GetScancodeName(event.key.keysym.scancode),
            "alt", (event.key.keysym.mod & KMOD_ALT) > 0,
            "ctrl", (event.key.keysym.mod & KMOD_CTRL) > 0,
            "shift", (event.key.keysym.mod & KMOD_SHIFT) > 0,
            "gui", (event.key.keysym.mod & KMOD_GUI) > 0,
            "capslock", (event.key.keysym.mod & KMOD_CAPS) > 0,
            "numlock", (event.key.keysym.mod & KMOD_NUM) > 0,
            "scrolllock", (event.key.keysym.mod & KMOD_SCROLL) > 0);

          break;
        case SDL_TEXTINPUT:
          event_type = "text_input";
          event_data.set("text", std::string(event.text.text));
          
          break;
      }
    }

    if (!captureMouse)
    {
      glm::ivec2 mouse_position;
      glm::ivec2 mouse_rowcol;

      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          mouse_position = glm::ivec2(event.button.x, event.button.y);
          mouse_rowcol = utility::GetRowColFromPosition(game_window, mouse_position);

          event_type = event.type == SDL_MOUSEBUTTONDOWN ? "mouse_down" : "mouse_up";
          event_data.set(
            "button", event.button.button,
            "clicks", event.button.clicks,
            "position", mouse_position,
            "rowcol", mouse_rowcol);
          
          break;
        case SDL_MOUSEMOTION:
          mouse_position = glm::ivec2(event.motion.x, event.motion.y);
          mouse_rowcol = utility::GetRowColFromPosition(game_window, mouse_position);

          event_type = "mouse_move";
          event_data.set(
            "movement", glm::ivec2(event.motion.xrel, event.motion.yrel),
            "position", mouse_position,
            "rowcol", mouse_rowcol
          );

          break;
        case SDL_MOUSEWHEEL:
          event_type = "mouse_scroll";
          event_data.set("movement", glm::vec2(event.wheel.preciseX, event.wheel.preciseY));

          break;
      }
    }

    if (event.type == SDL_WINDOWEVENT)
    {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_FOCUS_GAINED:  event_type = "window_keyboard_focus"; break;
        case SDL_WINDOWEVENT_FOCUS_LOST:    event_type = "window_keyboard_blur";  break;
        case SDL_WINDOWEVENT_ENTER:         event_type = "window_mouse_focus";    break;
        case SDL_WINDOWEVENT_LEAVE:         event_type = "window_mouse_blur";     break;
        case SDL_WINDOWEVENT_HIDDEN:        event_type = "window_hidden";         break;
        case SDL_WINDOWEVENT_SHOWN:         event_type = "window_shown";          break;
        case SDL_WINDOWEVENT_MINIMIZED:     event_type = "window_minimised";      break;
        case SDL_WINDOWEVENT_MAXIMIZED:     event_type = "window_maximised";      break;
        case SDL_WINDOWEVENT_RESTORED:      event_type = "window_restored";       break;
        case SDL_WINDOWEVENT_CLOSE:         event_type = "window_close";          break;
        case SDL_WINDOWEVENT_MOVED:
          event_type = "window_moved";
          event_data.set("position", glm::ivec2(event.window.data1, event.window.data2));

          break;
        case SDL_WINDOWEVENT_RESIZED:
          event_type = "window_resized";
          event_data.set("size", glm::ivec2(event.window.data1, event.window.data2));

          break;
      }
    }

    event_list.emplace_back(event_type, game_window, event_data);
  }
}