#include "Actions.h"

namespace term_engine::utilities {
  Action::Action() :
    action_(""),
    key_(nullptr) {}

  Action::Action(const std::string& action) :
    action_(action),
    key_(nullptr) {}

  std::string Action::GetAction() const {
    return action_;
  }

  SDL_Keycode Action::GetKey() const {
    return (*key_).first;
  }

  void Action::SetKey(const Key& key) {
    key_ = std::make_shared<Key>(key);
  }

  void Action::UnsetKey() {
    key_ = nullptr;
  }

  bool Action::IsActive() {
    return (*key_).second;
  }
}
