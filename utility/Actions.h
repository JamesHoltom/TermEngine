/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_ACTION_H
#define UTIL_ACTION_H

#include <memory>
#include <string>
#include <unordered_map>

#include "SDLIncludes.h"

namespace term_engine::utilities {
  typedef std::pair<SDL_Keycode, bool> Key;
  typedef std::shared_ptr<Key> KeyPtr;
  typedef std::unordered_map<SDL_Keycode, KeyPtr> KeyMap;
  
  class Action {
  public:
    Action();
    Action(const std::string& action);

    std::string GetAction() const;
    SDL_Keycode GetKey() const;
    void SetKey(const KeyPtr& key);
    void UnsetKey();

    bool IsActive() const;

  private:
    std::string action_;
    KeyPtr key_;
  };

  typedef std::unordered_map<std::string, Action> ActionMap;
}

#endif
