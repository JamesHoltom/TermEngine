#ifndef UTIL_BINDING_H
#define UTIL_BINDING_H

#include <string>
#include <unordered_map>

#include "../SDLIncludes.h"

namespace term_engine::utilities {

  class Binding {
  public:
    Binding();
    Binding(const std::string& action);

    std::string GetAction() const;
    void SetAction(const std::string& action);

    Uint16 GetModifiers() const;
    void SetModifiers(const Uint16& modifiers);
    bool CheckModifiers() const;

  protected:
    std::string action_;
    Uint16 modifiers_;
  };

  typedef std::unordered_map<std::string, Binding*> BindingMap;
}

#endif // !UTIL_BINDING_H

