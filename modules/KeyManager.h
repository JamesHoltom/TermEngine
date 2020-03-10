/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */
 
#ifndef MOD_KEY_MANAGER_H
#define MOD_KEY_MANAGER_H

#include <string>

#include "../utility/KeyBinding.h"
#include "../utility/SDLIncludes.h"

namespace term_engine::modules {
  class KeyManager {
    public:
      ~KeyManager();

      void HandleEvent(const SDL_Event& event);
      void UpdateFrames();
      
      bool RegisterAction(const std::string& action, const SDL_Keycode& key, const Uint16& modifiers = KMOD_NONE);
      bool UnregisterAction(const std::string& action);
      
      bool ReassignAction(const std::string& action, const SDL_Keycode& key);
      bool UnassignAction(const std::string& action);
      
      bool SetModifiers(const std::string& action, const Uint16& modifiers);
      bool ClearModifiers(const std::string& action);

      bool GetKeyDown(const std::string& action) const;
      bool GetKeyPressed(const std::string& action) const;
      bool GetKeyReleased(const std::string& action) const;

      int GetKeyFramesHeld(const std::string& action) const;
    private:
      utilities::KeyPtr RegisterKey(const SDL_Keycode& key);
      utilities::BindingMap actions_;
      utilities::KeyMap keys_;
  };
}

#endif // ! MOD_KEY_MANAGER_H
