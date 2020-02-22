#include <algorithm>

#include "KeyActions.h"

namespace term_engine::utilities {
  Key::Key() :
    key_(SDLK_UNKNOWN),
    is_pressed_(false) {}

  Key::Key(const SDL_Keycode& key) :
    key_(key),
    is_pressed_(false) {}

  SDL_Keycode Key::GetKey() const {
    return key_;
  }

  bool Key::IsPressed() const {
    return is_pressed_;
  }

  void Key::KeyPress() {
    is_pressed_ = true;
  }

  void Key::KeyRelease() {
    is_pressed_ = false;
  }

  Action::Action() :
    action_(""),
    key_(nullptr) {}

  Action::Action(const std::string& action) :
    action_(action),
    key_(nullptr) {}

  std::string Action::GetAction() const {
    return action_;
  }

  Key Action::GetKey() {
    return *key_;
  }

  void Action::SetKey(const Key& key) {
    key_ = std::make_shared<Key>(key);
  }

  void Action::UnsetKey() {
    key_ = nullptr;
  }

  bool Action::IsActive() const {
    return key_->IsPressed();
  }
}