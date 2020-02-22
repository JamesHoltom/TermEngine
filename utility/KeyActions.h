/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_KEY_H
#define UTIL_KEY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "SDLIncludes.h"

namespace term_engine::utilities {
  class Key {
  public:
    Key();
    Key(const SDL_Keycode& key);

    SDL_Keycode GetKey() const;

    bool IsPressed() const;

    void KeyPress();
    void KeyRelease();

  private:
    SDL_Keycode key_;
    bool is_pressed_;
  };

  typedef std::shared_ptr<Key> KeyPtr;

  class Action {
  public:
    Action();
    Action(const std::string& action);

    std::string GetAction() const;
    Key GetKey();
    void SetKey(const Key& key);
    void UnsetKey();

    bool IsActive() const;

  private:
    std::string action_;
    KeyPtr key_;
  };

  typedef std::unordered_map<std::string, Action> ActionMap;
  typedef std::unordered_map<SDL_Keycode, Key> KeyMap;
}

#endif
