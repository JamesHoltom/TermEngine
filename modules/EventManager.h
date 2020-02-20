/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */
 
#ifndef MOD_EVENT_MANAGER_H
#define MOD_EVENT_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utility/SDLIncludes.h"

namespace term_engine::modules {
  typedef struct _Action {
    std::string action_name;
    bool is_active;
  } Action;
  
  typedef std::shared_ptr<Action> ActionPtr;
  typedef std::unordered_map<std::string, ActionPtr> ActionMap;
  typedef std::unordered_map<SDL_Keycode, std::vector<ActionPtr>> KeyMap;
  
  class EventManager {
    public:
      void HandleEvents();
      
      int RegisterAction(const std::string& action);
      int UnregisterAction(const std::string& action);
      
      int RegisterKey(const SDL_Keycode& key);
      int UnregisterKey(const SDL_Keycode& key);
      
      int AssignKey(const SDL_Keycode& key, const std::string& action);
      int UnassignKey(const SDL_Keycode& key, const std::string& action);
    private:
      ActionMap actions_;
      KeyMap keys_;
  };
}

#endif
