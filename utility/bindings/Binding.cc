#include "Binding.h"

namespace term_engine::utilities {
  Binding::Binding() :
  action_(""),
  modifiers_(KMOD_NONE) {}

  Binding::Binding(const std::string& action) :
  action_(action),
  modifiers_(KMOD_NONE) {}

  std::string Binding::GetAction() const {
    return action_;
  }

  Uint16 Binding::GetModifiers() const {
    return modifiers_;
  }

  void Binding::SetModifiers(const Uint16& modifiers) {
    modifiers_ = modifiers;
  }

  bool Binding::CheckModifiers() const {
    Uint16 mods = SDL_GetModState();
    return (modifiers_ & mods) == modifiers_;
  }
}
