#include "KeyBinding.h"

namespace term_engine::utilities {
  KeyBinding::KeyBinding(const std::string& action) :
    action_(action),
    modifiers_(KMOD_NONE) {}

  std::string KeyBinding::GetAction() const {
    return action_;
  }

  SDL_Keycode KeyBinding::GetKey() const {
    if (auto tmpKey = key_.lock()) {
      return tmpKey->key;
    }

    return SDLK_UNKNOWN;
  }

  void KeyBinding::SetKey(KeyPtr key) {
    key_ = key;
  }

  void KeyBinding::UnsetKey() {
    key_.reset();
  }

  Uint16 KeyBinding::GetModifiers() const {
    return modifiers_;
  }

  void KeyBinding::SetModifiers(const Uint16& modifiers) {
    modifiers_ = modifiers;
  }

  bool KeyBinding::CheckModifiers() const {
    if (auto tmpKey = key_.lock()) {
      Uint16 mods = SDL_GetModState();
      return (modifiers_ & mods) == modifiers_;
    }

    return false;
  }

  bool KeyBinding::IsDown() const {
    if (auto tmpKey = key_.lock()) {
      return tmpKey->is_held;
    }

    return false;
  }

  bool KeyBinding::JustPressed() const {
    if (auto tmpKey = key_.lock()) {
      return tmpKey->is_held && tmpKey->held_frames == 0;
    }

    return false;
  }

  bool KeyBinding::JustReleased() const {
    if (auto tmpKey = key_.lock()) {
      return !tmpKey->is_held && tmpKey->held_frames > 0;
    }

    return false;
  }

  int KeyBinding::GetFramesHeld() const {
    if (auto tmpKey = key_.lock()) {
      return tmpKey->held_frames;
    }

    return 0;
  }
}
