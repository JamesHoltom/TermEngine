/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */
 
#ifndef MOD_EVENT_MANAGER_H
#define MOD_EVENT_MANAGER_H

#include <string>

#include "../utility/Actions.h"
#include "../utility/SDLIncludes.h"

namespace term_engine::modules {
  class InputManager {
    public:
      void HandleEvent(const SDL_Event& event);
      
      int RegisterAction(const std::string& action);
      int UnregisterAction(const std::string& action);
      
      int RegisterKey(const SDL_Keycode& key);
      int UnregisterKey(const SDL_Keycode& key);
      
      int AssignToAction(const SDL_Keycode& key, const std::string& action);
      int UnassignFromAction(const std::string& action);

      bool GetActionState(const std::string& action);
    private:
      utilities::ActionMap actions_;
      utilities::KeyMap keys_;
  };
}

#endif
