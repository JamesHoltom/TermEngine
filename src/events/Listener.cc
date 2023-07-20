#include <glm/glm.hpp>
#include "Listener.h"
#include "../scripting/ScriptingInterface.h"
#include "../system/CLArguments.h"
#include "../usertypes/GameScene.h"
#include "../utility/ConversionUtils.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::events {
  EventList event_list;
  
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
    bool captureKey = false;
    bool captureMouse = false;
    usertypes::GameScene* game_scene = nullptr;

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
          game_scene = usertypes::GetGameSceneByWindowId(event.key.windowID);

          if (game_scene != nullptr)
          {
            std::string event_type = event.type == SDL_KEYDOWN ? "key_down" : "key_up";

            event_list.emplace_back(event_type, game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "key", SDL_GetScancodeName(event.key.keysym.scancode),
              "alt", (event.key.keysym.mod & KMOD_ALT) > 0,
              "ctrl", (event.key.keysym.mod & KMOD_CTRL) > 0,
              "shift", (event.key.keysym.mod & KMOD_SHIFT) > 0,
              "gui", (event.key.keysym.mod & KMOD_GUI) > 0,
              "capslock", (event.key.keysym.mod & KMOD_CAPS) > 0,
              "numlock", (event.key.keysym.mod & KMOD_NUM) > 0,
              "scrolllock", (event.key.keysym.mod & KMOD_SCROLL) > 0
            ));
          }
          break;
        case SDL_TEXTINPUT:
          game_scene = usertypes::GetGameSceneByWindowId(event.text.windowID);

          if (game_scene != nullptr)
          {
            event_list.emplace_back("text_input", game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "text", std::string(event.text.text)
            ));
          }
          break;
        case SDL_TEXTEDITING:
          game_scene = usertypes::GetGameSceneByWindowId(event.edit.windowID);

          if (game_scene != nullptr)
          {
            event_list.emplace_back("text_editing", game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "text", std::string(event.edit.text),
              "cursor_start", event.edit.start,
              "cursor_length", event.edit.length
            ));
          }
          break;
      }
    }

    if (!captureMouse)
    {
      switch (event.type)
      {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          game_scene = usertypes::GetGameSceneByWindowId(event.button.windowID);

          if (game_scene != nullptr)
          {
            std::string event_type = event.type == SDL_MOUSEBUTTONDOWN ? "mouse_down" : "mouse_up";
            glm::ivec2 mouse_position = glm::ivec2(event.button.x, event.button.y);
            glm::ivec2 mouse_rowcol = utility::GetRowColFromPosition(game_scene, mouse_position);

            event_list.emplace_back(event_type, game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "button", event.button.button,
              "clicks", event.button.clicks,
              "position", mouse_position,
              "rowcol", mouse_rowcol
            ));
          }
          break;
        case SDL_MOUSEMOTION:
          game_scene = usertypes::GetGameSceneByWindowId(event.button.windowID);

          if (game_scene != nullptr)
          {
            glm::ivec2 mouse_position = glm::ivec2(event.motion.x, event.motion.y);
            glm::ivec2 mouse_rowcol = utility::GetRowColFromPosition(game_scene, mouse_position);

            event_list.emplace_back("mouse_move", game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "movement", glm::ivec2(event.motion.xrel, event.motion.yrel),
              "position", mouse_position,
              "rowcol", mouse_rowcol
            ));
          }
          break;
        case SDL_MOUSEWHEEL:
          event_list.emplace_back("mouse_scroll", game_scene, scripting::lua_state->create_table_with(
            "game_scene", game_scene->GetName(),
            "movement", glm::vec2(event.wheel.preciseX, event.wheel.preciseY)
          ));
          break;
      }
    }

    if (event.type == SDL_WINDOWEVENT)
    {
      game_scene = usertypes::GetGameSceneByWindowId(event.window.windowID);

      if (game_scene != nullptr)
      {
        sol::table window_data = scripting::lua_state->create_table_with("game_scene", game_scene->GetName());

        switch (event.window.event) {
          case SDL_WINDOWEVENT_FOCUS_GAINED:  event_list.emplace_back("window_keyboard_focus", game_scene, window_data); break;
          case SDL_WINDOWEVENT_FOCUS_LOST:    event_list.emplace_back("window_keyboard_blur", game_scene, window_data);  break;
          case SDL_WINDOWEVENT_ENTER:         event_list.emplace_back("window_mouse_focus", game_scene, window_data);    break;
          case SDL_WINDOWEVENT_LEAVE:         event_list.emplace_back("window_mouse_blur", game_scene, window_data);     break;
          case SDL_WINDOWEVENT_HIDDEN:        event_list.emplace_back("window_hidden", game_scene, window_data);         break;
          case SDL_WINDOWEVENT_SHOWN:         event_list.emplace_back("window_shown", game_scene, window_data);          break;
          case SDL_WINDOWEVENT_MINIMIZED:     event_list.emplace_back("window_minimised", game_scene, window_data);      break;
          case SDL_WINDOWEVENT_MAXIMIZED:     event_list.emplace_back("window_maximised", game_scene, window_data);      break;
          case SDL_WINDOWEVENT_RESTORED:      event_list.emplace_back("window_restored", game_scene, window_data);       break;
          case SDL_WINDOWEVENT_CLOSE:         event_list.emplace_back("window_close", game_scene, window_data);          break;
          case SDL_WINDOWEVENT_MOVED:
            event_list.emplace_back("window_moved", game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "position", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
          case SDL_WINDOWEVENT_RESIZED:
            event_list.emplace_back("window_resized", game_scene, scripting::lua_state->create_table_with(
              "game_scene", game_scene->GetName(),
              "size", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
        }
      }
    }
  }
}