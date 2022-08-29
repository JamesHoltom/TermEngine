#include <algorithm>
#include <glm/glm.hpp>
#include "Listener.h"
#include "../data/Uniform.h"
#include "../logging/Logger.h"
#include "../scripting/ScriptingInterface.h"

namespace term_engine::events {
  EventListener::EventListener(const std::string& type, const sol::function func) :
    type_(type),
    func_(sol::make_reference<sol::function>((*scripting::lua_state), func))
  {
    logging::logger->debug("Created event listener of type {}.", type);
  }
  
  std::string EventListener::GetType() const
  {
    return type_;
  }

  bool EventListener::IsActive() const
  {
    return active_;
  }

  void EventListener::SetActive(const bool& active)
  {
    active_ = active;
  }

  void EventListener::Trigger(const sol::table& data)
  {
    func_(data);
  }

  ListenerPtr& EventListener::AddEventListener(const std::string& type, const sol::function func)
  {
    if (listeners_.find(type) == listeners_.end())
    {
      ListenerList list;
      listeners_.insert_or_assign(type, list);
    }

    listeners_[type].push_back(std::make_shared<EventListener>(type, func));

    return listeners_[type].back();
  }

  void EventListener::RemoveEventListener(ListenerPtr& event)
  {
    bool found = false;

    if (listeners_.find(event->type_) != listeners_.end())
    {
      ListenerList::iterator it = std::find(listeners_.at(event->type_).begin(), listeners_.at(event->type_).end(), event);

      if (it != listeners_[event->type_].end())
      {
        listeners_[event->type_].erase(it);
        found = true;
      }
    }

    if (!found)
    {
      logging::logger->warn("Could not find function in list of event listeners for {}.", event->type_);
    }

    event.reset();
  }

  void EventListener::ClearEventListeners(const std::string& type)
  {
    listeners_.erase(type);
  }

  int EventListener::CountListeners(const std::string& type)
  {
    if (listeners_.find(type) != listeners_.end())
    {
      return listeners_[type].size();
    }
    else
    {
      return 0;
    }
  }

  std::vector<std::string> EventListener::GetListenerTypes()
  {
    std::vector<std::string> types(listeners_.size());

    std::transform(listeners_.begin(), listeners_.end(), types.begin(), [](auto pair) { return pair.first; });

    return types;
  }

  void EventListener::Dispatch(const std::string& type, const sol::table& data)
  {
    if (listeners_.find(type) != listeners_.end())
    {
      for (const ListenerPtr& it : listeners_[type])
      {
        if (it->active_)
        {
          it->func_(it, data);
        }
      }
    }
  }

  void EventListener::DoSDLEvents(const SDL_Event& event)
  {
    switch (event.type)
    {
      case SDL_KEYDOWN:
        Dispatch("key_down", scripting::lua_state->create_table_with(
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
        Dispatch("key_up", scripting::lua_state->create_table_with(
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
        Dispatch("mouse_down", scripting::lua_state->create_table_with(
          "button", event.button.button,
          "clicks", event.button.clicks,
          "position", glm::ivec2(event.button.x, event.button.y)
        ));
        break;
      case SDL_MOUSEBUTTONUP:
        Dispatch("mouse_up", scripting::lua_state->create_table_with(
          "button", event.button.button,
          "clicks", event.button.clicks,
          "position", glm::ivec2(event.button.x, event.button.y)
        ));
        break;
      case SDL_MOUSEMOTION:
        Dispatch("mouse_move", scripting::lua_state->create_table_with(
          "movement", glm::ivec2(event.motion.xrel, event.motion.yrel),
          "position", glm::ivec2(event.motion.x, event.motion.y)
        ));
        break;
      case SDL_MOUSEWHEEL:
        Dispatch("mouse_scroll", scripting::lua_state->create_table_with(
          "movement", glm::vec2(event.wheel.preciseX, event.wheel.preciseY)
        ));
        break;
//      case SDL_TEXTINPUT:                 Dispatch("text_input", {});             break;
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_ENTER:         Dispatch("window_mouse_focus", {});     break;
          case SDL_WINDOWEVENT_LEAVE:         Dispatch("window_mouse_blur", {});      break;
          case SDL_WINDOWEVENT_FOCUS_GAINED:  Dispatch("window_keyboard_focus", {});  break;
          case SDL_WINDOWEVENT_FOCUS_LOST:    Dispatch("window_keyboard_blur", {});   break;
          case SDL_WINDOWEVENT_HIDDEN:        Dispatch("window_hidden", {});          break;
          case SDL_WINDOWEVENT_SHOWN:         Dispatch("window_shown", {});           break;
          case SDL_WINDOWEVENT_MAXIMIZED:     Dispatch("window_maximised", {});       break;
          case SDL_WINDOWEVENT_MINIMIZED:     Dispatch("window_minimised", {});       break;
          case SDL_WINDOWEVENT_MOVED:
            Dispatch("window_moved", scripting::lua_state->create_table_with(
              "position", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
          case SDL_WINDOWEVENT_RESIZED:
            Dispatch("window_resized", scripting::lua_state->create_table_with(
              "size", glm::ivec2(event.window.data1, event.window.data2)
            ));
            break;
          case SDL_WINDOWEVENT_RESTORED:      Dispatch("window_restored", {});        break;
      }
    }
  }

  void EventListener::CleanUp()
  {
    for (auto& listener : listeners_)
    {
      listener.second.clear();
    }

    listeners_.clear();
  }

  ListenerMap EventListener::listeners_;
}