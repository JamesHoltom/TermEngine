#include "EventManager.h"

namespace term_engine::modules {
  void EventManager::HandleEvents() {
    SDL_Event evt;
    
    while (SDL_PollEvent(&evt) != 0) {
      switch (evt.type) {
        case SDL_KEYDOWN:
          
          break;
        case SDL_KEYUP:
          
          break;
      }
    }
  }
  
  int EventManager::RegisterAction(const std::string& action) {
    Action new_action;
    new_action.action_name = action;
    new_action.is_active = false;
    
    bool result = actions_.emplace(action, std::make_shared<Action>(new_action)).second;
    
    if (result) {
      printf("Registered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Failed to register action!\n");
    return -1;
  }
  
  int EventManager::UnregisterAction(const std::string& action) {
    actions_.at(action).reset();
    bool result = actions_.erase(action) == 1;
    
    if (result) {
      printf("Unregistered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Failed to unregister action!\n");
    return -1;
  }
  
  int EventManager::RegisterKey(const SDL_Keycode& key) {
    bool result = keys_.emplace(key, std::vector<ActionPtr>(0)).second;
    
    if (result) {
      printf("Registered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Failed to register key!\n");
    return -1;
  }
  
  int EventManager::UnregisterKey(const SDL_Keycode& key) {
    bool result = keys_.erase(key) == 1;
    
    if (result) {
      printf("Unregistered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Failed to unregister key!\n");
    return -1;
  }
  
  int EventManager::AssignKey(const SDL_Keycode& key, const std::string& action) {
    return 0;
  }
  
  int EventManager::UnassignKey(const SDL_Keycode& key, const std::string& action) {
    return 0;
  }
}
