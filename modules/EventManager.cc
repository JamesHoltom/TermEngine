
#include "EventManager.h"

namespace term_engine::modules {
  void EventManager::HandleEvents() {
    SDL_Event evt;
    
    while (SDL_PollEvent(&evt) != 0) {
      SDL_Keycode key = evt.key.keysym.sym;
      auto key_it = keys_.find(key);

      if (key_it != keys_.end()) {
        switch (evt.type) {
        case SDL_KEYDOWN:
          keys_[key].KeyPress();

          break;
        case SDL_KEYUP:
          keys_[key].KeyRelease();

          break;
        }
      }
    }
  }
  
  int EventManager::RegisterAction(const std::string& action) {
    utilities::Action new_action(action);
    
    bool result = actions_.emplace(action, new_action).second;
    
    if (result) {
      printf("Registered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Action \'%s\' already exists!\n", action.c_str());
    return -1;
  }
  
  int EventManager::UnregisterAction(const std::string& action) {
    bool result = actions_.erase(action) == 1;
    
    if (result) {
      printf("Unregistered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Action \'%s\' does not exist!\n", action.c_str());
    return -1;
  }
  
  int EventManager::RegisterKey(const SDL_Keycode& key) {
    utilities::Key new_key(key);

    bool result = keys_.emplace(key, new_key).second;
    
    if (result) {
      printf("Registered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Key \'%i\' already exists!\n", key);
    return -1;
  }
  
  int EventManager::UnregisterKey(const SDL_Keycode& key) {
    bool result = keys_.erase(key) == 1;
    
    if (result) {
      printf("Unregistered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Key \'%i\' does not exist!\n", key);
    return -1;
  }
  
  int EventManager::AssignToAction(const SDL_Keycode& key, const std::string& action) {
    auto key_it = keys_.find(key);
    auto action_it = actions_.find(action);

    if (key_it != keys_.end() && action_it != actions_.end()) {
      action_it->second.SetKey(key_it->second);

      printf("Assigned key \'%i\' to action \'%s\'.", key, action.c_str());
      return 0;
    }

    printf("Failed to assign key \'%i\' to action \'%s\'!", key, action.c_str());
    return -1;
  }
  
  int EventManager::UnassignFromAction(const std::string& action) {
    auto action_it = actions_.find(action);

    if (action_it != actions_.end()) {
      SDL_Keycode key = action_it->second.GetKey().GetKey();
      action_it->second.UnsetKey();

      printf("Assigned key \'%i\' to action \'%s\'.", key, action.c_str());
      return 0;
    }

    printf("Failed to assign key from action \'%s\'!", action.c_str());
    return -1;
  }

  bool EventManager::GetActionState(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.IsActive();
    }

    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }
}
