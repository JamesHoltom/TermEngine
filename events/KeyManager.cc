#include <algorithm>
#include <memory>

#include "KeyManager.h"

namespace term_engine::modules {
  KeyManager::~KeyManager() {
    keys_.clear();
    actions_.clear();
  }

  void KeyManager::HandleEvent(const SDL_Event& event) {
    SDL_Keycode key = event.key.keysym.sym;
    auto key_it = keys_.find(key);

    if (key_it != keys_.end()) {
      switch (event.type) {
      case SDL_KEYDOWN:
        keys_[key]->is_held = true;

        break;
      case SDL_KEYUP:
        keys_[key]->is_held = false;

        break;
      }
    }
  }

  void KeyManager::UpdateFrames() {
    std::for_each(keys_.begin(), keys_.end(), [](std::pair<SDL_Keycode, utilities::KeyPtr> key) {
      if (key.second->is_held) {
        key.second->held_frames++;
      }
      else {
        key.second->held_frames = 0;
      }
    });
  }
  
  bool KeyManager::RegisterAction(const std::string& action, const SDL_Keycode& key, const Uint16& modifiers) {
    utilities::KeyPtr new_key = RegisterKey(key);
    utilities::KeyBinding new_action(action);
    new_action.SetKey(new_key);
    new_action.SetModifiers(modifiers);
    
    const bool& result = actions_.emplace(action, new_action).second;
    
    if (result) {
      printf("Registered action \'%s\'.\n", action.c_str());
      return true;
    }
    
    printf("Action \'%s\' already exists!\n", action.c_str());
    return false;
  }
  
  bool KeyManager::UnregisterAction(const std::string& action) {
    const bool& result = actions_.erase(action) == 1;
    
    if (result) {
      printf("Unregistered action \'%s\'.\n", action.c_str());
      return true;
    }
    
    printf("Action \'%s\' does not exist!\n", action.c_str());
    return false;
  }
  
  bool KeyManager::ReassignAction(const std::string& action, const SDL_Keycode& key) {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      auto new_key = RegisterKey(key);
      it->second.SetKey(new_key);

      printf("Assigned key \'%i\' to action \'%s\'.\n", key, action.c_str());
      return true;
    }

    printf("Failed to assign key \'%i\' to action \'%s\'!\n", key, action.c_str());
    return false;
  }
  
  bool KeyManager::UnassignAction(const std::string& action) {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      SDL_Keycode key = it->second.GetKey();
      it->second.UnsetKey();

      printf("Assigned key \'%i\' to action \'%s\'.\n", key, action.c_str());
      return true;
    }

    printf("Failed to assign key from action \'%s\'!\n", action.c_str());
    return false;
  }
  
  bool KeyManager::SetModifiers(const std::string& action, const Uint16& modifiers) {
    auto it = actions_.find(action);
    
    if (it != actions_.end()) {
      it->second.SetModifiers(modifiers);
      return true;
    }
    
    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }
  
  bool KeyManager::ClearModifiers(const std::string& action) {
    auto it = actions_.find(action);
    
    if (it != actions_.end()) {
      it->second.SetModifiers(KMOD_NONE);
      return true;
    }
    
    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }

  bool KeyManager::GetKeyDown(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.IsDown() && it->second.CheckModifiers();
    }

    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }

  bool KeyManager::GetKeyPressed(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.JustPressed() && it->second.CheckModifiers();
    }

    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }

  bool KeyManager::GetKeyReleased(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.JustReleased() && it->second.CheckModifiers();
    }

    printf("Could not find action \'%s\'!\n", action.c_str());
    return false;
  }

  int KeyManager::GetKeyFramesHeld(const std::string& action) const {
    auto it = actions_.find(action);

    if (it != actions_.end()) {
      return it->second.GetFramesHeld();
    }

    printf("Could not find action \'%s\'\n", action.c_str());
    return -1;
  }
  
  utilities::KeyPtr KeyManager::RegisterKey(const SDL_Keycode& key) {
    utilities::KeyPtr new_key = std::make_shared<utilities::Key>(utilities::Key(key));
    
    auto result = keys_.emplace(key, new_key);
    
    if (result.second) {
      printf("Registered key \'%i\'.\n", key);
    } else {
      printf("Key \'%i\' already exists!\n", key);
    }
    
    return result.first->second;
  }
}
