/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_KEY_BINDING_H
#define UTIL_KEY_BINDING_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../includes.h"

namespace term_engine::utilities {
  struct Key {
    SDL_Keycode key;
    bool is_held;
    int held_frames;
    Key(const SDL_Keycode& key) : key(key), is_held(false), held_frames(0) {}
  };

  typedef std::shared_ptr<Key> KeyPtr;
  typedef std::weak_ptr<Key> KeyWeakPtr;
  typedef std::unordered_map<SDL_Keycode, KeyPtr> KeyMap;

  class KeyBinding {
  public:
    KeyBinding(const std::string& action);

    std::string GetAction() const;

    SDL_Keycode GetKey() const;
    void SetKey(KeyPtr key);
    void UnsetKey();

    Uint16 GetModifiers() const;
    void SetModifiers(const Uint16& modifiers);
    bool CheckModifiers() const;

    bool IsDown() const;
    bool JustPressed() const;
    bool JustReleased() const;

    int GetFramesHeld() const;

  private:
    std::string action_;
    Uint16 modifiers_;
    KeyWeakPtr key_;
  };

  typedef std::unordered_map<std::string, KeyBinding> BindingMap;
}

#endif // ! UTIL_KEY_BINDING_H
