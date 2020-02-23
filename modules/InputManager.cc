#include <memory>

#include "InputManager.h"

namespace term_engine::modules {
  InputManager::~InputManager() {
    keys_.clear();
    actions_.clear();
  }

  void InputManager::HandleEvent(const SDL_Event& event) {
    SDL_Keycode key = event.key.keysym.sym;
    auto key_it = keys_.find(key);

    if (key_it != keys_.end()) {
      switch (event.type) {
      case SDL_KEYDOWN:
        keys_[key]->second = true;

        break;
      case SDL_KEYUP:
        keys_[key]->second = false;

        break;
      }
    }
  }
  
  int InputManager::RegisterAction(const std::string& action) {
    utilities::Action new_action(action);
    
    const bool& result = actions_.emplace(action, new_action).second;
    
    if (result) {
      printf("Registered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Action \'%s\' already exists!\n", action.c_str());
    return -1;
  }
  
  int InputManager::UnregisterAction(const std::string& action) {
    const bool& result = actions_.erase(action) == 1;
    
    if (result) {
      printf("Unregistered action \'%s\'.\n", action.c_str());
      return 0;
    }
    
    printf("Action \'%s\' does not exist!\n", action.c_str());
    return -1;
  }
  
  int InputManager::RegisterKey(const SDL_Keycode& key) {
    utilities::KeyPtr new_key = std::make_shared<utilities::Key>(std::make_pair(key, false));
    
    const bool& result = keys_.emplace(key, new_key).second;
    
    if (result) {
      printf("Registered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Key \'%i\' already exists!\n", key);
    return -1;
  }
  
  int InputManager::UnregisterKey(const SDL_Keycode& key) {
    const bool& result = keys_.erase(key) == 1;
    
    if (result) {
      printf("Unregistered key \'%i\'.\n", key);
      return 0;
    }
    
    printf("Key \'%i\' does not exist!\n", key);
    return -1;
  }
  
  int InputManager::RegisterAndAssign(const SDL_Keycode& key, const std::string& action) {
    int key_result = RegisterKey(key);
    int action_result = RegisterAction(action);

    if (key_result == 0 && action_result == 0) {
      return AssignToAction(key, action);
    }

    return -1;
  }

  int InputManager::AssignToAction(const SDL_Keycode& key, const std::string& action) {
    auto key_it = keys_.find(key);
    auto action_it = actions_.find(action);

    if (key_it != keys_.end() && action_it != actions_.end()) {
      action_it->second.SetKey(key_it->second);

      printf("Assigned key \'%i\' to action \'%s\'.\n", key, action.c_str());
      return 0;
    }

    printf("Failed to assign key \'%i\' to action \'%s\'!\n", key, action.c_str());
    return -1;
  }
  
  int InputManager::UnassignFromAction(const std::string& action) {
    auto action_it = actions_.find(action);

    if (action_it != actions_.end()) {
      SDL_Keycode key = action_it->second.GetKey();
      action_it->second.UnsetKey();

      printf("Assigned key \'%i\' to action \'%s\'.\n", key, action.c_str());
      return 0;
    }

    printf("Failed to assign key from action \'%s\'!\n", action.c_str());
    return -1;
  }

  bool InputManager::GetActionState(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.IsActive();
    }

    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }
}
