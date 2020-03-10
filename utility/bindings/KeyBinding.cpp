#include "KeyBinding.h"

namespace term_engine::utilities {
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
