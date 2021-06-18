/// @author James Holtom

#ifndef GLM_BINDINGS_H
#define GLM_BINDINGS_H

#include <string>

#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  void BindGlmToState(sol::state& state)
  {
    state.new_usertype<glm::ivec2>(
      "ivec2",
      sol::call_constructor, sol::constructors<void(), void(const int&), void(const int&, const int&)>(),
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y,
      sol::meta_function::equal_to, sol::overload([](const glm::ivec2& lhs, const glm::ivec2& rhs) { return lhs == rhs; }),
      sol::meta_function::addition, sol::overload([](const glm::ivec2& lhs, const glm::ivec2& rhs) { return lhs + rhs; }),
      sol::meta_function::subtraction, sol::overload([](const glm::ivec2& lhs, const glm::ivec2& rhs) { return lhs - rhs; }),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) { return lhs * rhs; },
        [](const glm::ivec2& lhs, const int& rhs) { return lhs * rhs; },
        [](const int& lhs, const glm::ivec2& rhs) { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) { return lhs / rhs; },
        [](const glm::ivec2& lhs, const int& rhs) { return lhs / rhs; }),
      sol::meta_function::to_string, [&](const glm::ivec2& self) { return std::to_string(self.x) + ", " + std::to_string(self.y); });

    state.new_usertype<glm::vec2>(
      "vec2",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&)>(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y,
      sol::meta_function::equal_to, sol::overload([](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs == rhs; }),
      sol::meta_function::addition, sol::overload([](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs + rhs; }),
      sol::meta_function::subtraction, sol::overload([](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs - rhs; }),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs * rhs; },
        [](const glm::vec2& lhs, const float& rhs) { return lhs * rhs; },
        [](const float& lhs, const glm::vec2& rhs) { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs / rhs; },
        [](const glm::vec2& lhs, const float& rhs) { return lhs / rhs; }
      ),
      sol::meta_function::to_string, [&](const glm::vec2& self) { return std::to_string(self.x) + ", " + std::to_string(self.y); },
      "floor", [](const glm::vec2& self) { return glm::floor(self); });

    state.new_usertype<glm::vec3>(
      "vec3",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&, const float&)>(),
      "x", &glm::vec3::x,
      "y", &glm::vec3::y,
      "z", &glm::vec3::z,
      "r", &glm::vec3::r,
      "g", &glm::vec3::g,
      "b", &glm::vec3::b,
      sol::meta_function::equal_to, sol::overload([](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs == rhs; }),
      sol::meta_function::addition, sol::overload([](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs + rhs; }),
      sol::meta_function::subtraction, sol::overload([](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs - rhs; }),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs * rhs; },
        [](const glm::vec3& lhs, const float& rhs) { return lhs * rhs; },
        [](const float& lhs, const glm::vec3& rhs) { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs / rhs; },
        [](const glm::vec3& lhs, const float& rhs) { return lhs / rhs; }),
      sol::meta_function::to_string, [&](const glm::vec3& self) { return std::to_string(self.x) + ", " + std::to_string(self.y) + ", " + std::to_string(self.z); });
  }
}

#endif // ! GLM_BINDINGS_H
