/// @author James Holtom

#ifndef GLM_BINDINGS_H
#define GLM_BINDINGS_H

#include <string>
#include <glm/glm.hpp>
#include <glm/detail/compute_vector_relational.hpp>
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds GLM vector types as usertypes to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindGlmToState(sol::state& state)
  {
    state.new_usertype<glm::ivec2>(
      "Ivec2",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(int), void(int, int),
                                                       void(const glm::ivec2&), void(const glm::vec2&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(int), void(int, int),
                                               void(const glm::ivec2&), void(const glm::vec2&)>(),
      sol::meta_function::type, state.create_table_with("name", "Ivec2"),
      sol::meta_function::equal_to, [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> glm::ivec2 { return lhs + rhs; },
        [](const glm::ivec2& lhs, int rhs) -> glm::ivec2 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> glm::ivec2 { return lhs - rhs; },
        [](const glm::ivec2& lhs, int rhs) -> glm::ivec2 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> glm::ivec2 { return lhs * rhs; },
        [](const glm::ivec2& lhs, int rhs) -> glm::ivec2 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> glm::ivec2 { return lhs / rhs; },
        [](const glm::ivec2& lhs, int rhs) -> glm::ivec2 { return lhs / rhs; }
      ),
      sol::meta_function::less_than, [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::ivec2& self) -> glm::ivec2 { return -self; },
      sol::meta_function::to_string, [](const glm::ivec2& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y); },
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y,
      "abs", [](const glm::ivec2& self) -> glm::ivec2 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::ivec2& self, const glm::ivec2& val) -> glm::ivec2 { return glm::min(self, val); },
        [](const glm::ivec2& self, int val) -> glm::ivec2 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::ivec2& self, const glm::ivec2& val) -> glm::ivec2 { return glm::max(self, val); },
        [](const glm::ivec2& self, int val) -> glm::ivec2 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::ivec2& self, const glm::ivec2& min, const glm::ivec2& max) -> glm::ivec2 { return glm::clamp(self, min, max); },
        [](const glm::ivec2& self, int min, int max) -> glm::ivec2 { return glm::clamp(self, min, max); }
      ));

    state.new_usertype<glm::vec2>(
      "Vec2",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(float), void(float, float),
                                                       void(const glm::vec2&), void(const glm::ivec2&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(float), void(float, float),
                                               void(const glm::vec2&), void(const glm::ivec2&)>(),
      sol::meta_function::type, state.create_table_with("name", "Vec2"),
      sol::meta_function::equal_to, [](const glm::vec2& lhs, const glm::vec2& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) -> glm::vec2 { return lhs + rhs; },
        [](const glm::vec2& lhs, float rhs) -> glm::vec2 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) -> glm::vec2 { return lhs - rhs; },
        [](const glm::vec2& lhs, float rhs) -> glm::vec2 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) -> glm::vec2 { return lhs * rhs; },
        [](const glm::vec2& lhs, float rhs) -> glm::vec2 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) -> glm::vec2 { return lhs / rhs; },
        [](const glm::vec2& lhs, float rhs) -> glm::vec2 { return lhs / rhs; }
      ),
      sol::meta_function::less_than, [](const glm::vec2& lhs, const glm::vec2& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::vec2& lhs, const glm::vec2& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::vec2& self) -> glm::vec2 { return -self; },
      sol::meta_function::to_string, [](const glm::vec2& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y); },
      "x", &glm::vec2::x,
      "y", &glm::vec2::y,
      "floor", [](const glm::vec2& self) -> glm::vec2 { return glm::floor(self); },
      "ceil", [](const glm::vec2& self) -> glm::vec2 { return glm::ceil(self); },
      "round", [](const glm::vec2& self) -> glm::vec2 { return glm::round(self); },
      "abs", [](const glm::vec2& self) -> glm::vec2 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::vec2& self, const glm::vec2& val) -> glm::vec2 { return glm::min(self, val); },
        [](const glm::vec2& self, float val) -> glm::vec2 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::vec2& self, const glm::vec2& val) -> glm::vec2 { return glm::max(self, val); },
        [](const glm::vec2& self, float val) -> glm::vec2 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::vec2& self, const glm::vec2& min, const glm::vec2& max) -> glm::vec2 { return glm::clamp(self, min, max); },
        [](const glm::vec2& self, float min, float max) -> glm::vec2 { return glm::clamp(self, min, max); }
      ));

    state.new_usertype<glm::ivec3>(
      "Ivec3",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(int), void(int, int, int),
                                                       void(const glm::ivec2&, int), void(const glm::vec2&, float),
                                                       void(const glm::ivec3&), void(const glm::vec3&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(int), void(int, int, int),
                                               void(const glm::ivec2&, int), void(const glm::vec2&, float),
                                               void(const glm::ivec3&), void(const glm::vec3&)>(),
      sol::meta_function::type, state.create_table_with("name", "Ivec3"),
      sol::meta_function::equal_to, [](const glm::ivec3& lhs, const glm::ivec3& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::ivec3& lhs, const glm::ivec3& rhs) -> glm::ivec3 { return lhs + rhs; },
        [](const glm::ivec3& lhs, int rhs) -> glm::ivec3 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::ivec3& lhs, const glm::ivec3& rhs) -> glm::ivec3 { return lhs - rhs; },
        [](const glm::ivec3& lhs, int rhs) -> glm::ivec3 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::ivec3& lhs, const glm::ivec3& rhs) -> glm::ivec3 { return lhs * rhs; },
        [](const glm::ivec3& lhs, int rhs) -> glm::ivec3 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::ivec3& lhs, const glm::ivec3& rhs) -> glm::ivec3 { return lhs / rhs; },
        [](const glm::ivec3& lhs, int rhs) -> glm::ivec3 { return lhs / rhs; }
      ),
      sol::meta_function::less_than, [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::ivec2& lhs, const glm::ivec2& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::ivec2& self) -> glm::ivec2 { return -self; },
      sol::meta_function::to_string, [](const glm::ivec3& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y) + "," + std::to_string(self.z); },
      "x", &glm::ivec3::x,
      "y", &glm::ivec3::y,
      "z", &glm::ivec3::z,
      "r", &glm::ivec3::r,
      "g", &glm::ivec3::g,
      "b", &glm::ivec3::b,
      "abs", [](const glm::ivec3& self) -> glm::ivec3 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::ivec3& self, const glm::ivec3& val) -> glm::ivec3 { return glm::min(self, val); },
        [](const glm::ivec3& self, int val) -> glm::ivec3 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::ivec3& self, const glm::ivec3& val) -> glm::ivec3 { return glm::max(self, val); },
        [](const glm::ivec3& self, int val) -> glm::ivec3 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::ivec3& self, const glm::ivec3& min, const glm::ivec3& max) -> glm::ivec3 { return glm::clamp(self, min, max); },
        [](const glm::ivec3& self, int min, int max) -> glm::ivec3 { return glm::clamp(self, min, max); }
      ));

    state.new_usertype<glm::vec3>(
      "Vec3",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(float), void(float, float, float),
                                                       void(const glm::vec2&, float), void(const glm::ivec2&, int),
                                                       void(const glm::vec3&), void(const glm::ivec3&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(float), void(float, float, float),
                                               void(const glm::vec2&, float), void(const glm::ivec2&, int),
                                               void(const glm::vec3&), void(const glm::ivec3&)>(),
      sol::meta_function::type, state.create_table_with("name", "Vec3"),
      sol::meta_function::equal_to, [](const glm::vec3& lhs, const glm::vec3& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3 { return lhs + rhs; },
        [](const glm::vec3& lhs, float rhs) -> glm::vec3 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3 { return lhs - rhs; },
        [](const glm::vec3& lhs, float rhs) -> glm::vec3 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3 { return lhs * rhs; },
        [](const glm::vec3& lhs, float rhs) -> glm::vec3 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3 { return lhs / rhs; },
        [](const glm::vec3& lhs, float rhs) -> glm::vec3 { return lhs / rhs; }),
      sol::meta_function::less_than, [](const glm::vec3& lhs, const glm::vec3& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::vec3& lhs, const glm::vec3& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::vec3& self) -> glm::vec3 { return -self; },
      sol::meta_function::to_string, [](const glm::vec3& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y) + "," + std::to_string(self.z); },
      "x", &glm::vec3::x,
      "y", &glm::vec3::y,
      "z", &glm::vec3::z,
      "r", &glm::vec3::r,
      "g", &glm::vec3::g,
      "b", &glm::vec3::b,
      "floor", [](const glm::vec3& self) -> glm::vec3 { return glm::floor(self); },
      "ceil", [](const glm::vec3& self) -> glm::vec3 { return glm::ceil(self); },
      "round", [](const glm::vec3& self) -> glm::vec3 { return glm::round(self); },
      "abs", [](const glm::vec3& self) -> glm::vec3 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::vec3& self, const glm::vec3& val) -> glm::vec3 { return glm::min(self, val); },
        [](const glm::vec3& self, float val) -> glm::vec3 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::vec3& self, const glm::vec3& val) -> glm::vec3 { return glm::max(self, val); },
        [](const glm::vec3& self, float val) -> glm::vec3 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::vec3& self, const glm::vec3& min, const glm::vec3& max) -> glm::vec3 { return glm::clamp(self, min, max); },
        [](const glm::vec3& self, float min, float max) -> glm::vec3 { return glm::clamp(self, min, max); }
      ));

    state.new_usertype<glm::ivec4>(
      "Ivec4",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(int), void(int, int, int, int),
                                                       void(const glm::ivec2&, int, int), void(const glm::vec2&, int, int),
                                                       void(const glm::ivec2&, const glm::ivec2&), void(const glm::vec2&, const glm::vec2&),
                                                       void(const glm::ivec3&, int), void(const glm::vec3&, float),
                                                       void(const glm::ivec4&), void(const glm::vec4&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(int), void(int, int, int, int),
                                               void(const glm::ivec2&, int, int), void(const glm::vec2&, float, float),
                                               void(const glm::ivec2&, const glm::ivec2&), void(const glm::vec2&, const glm::vec2&),
                                               void(const glm::ivec3&, int), void(const glm::vec3&, float),
                                               void(const glm::ivec4&), void(const glm::vec4&)>(),
      sol::meta_function::type, state.create_table_with("name", "Ivec4"),
      sol::meta_function::equal_to, [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> glm::ivec4 { return lhs + rhs; },
        [](const glm::ivec4& lhs, int rhs) -> glm::ivec4 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> glm::ivec4 { return lhs - rhs; },
        [](const glm::ivec4& lhs, int rhs) -> glm::ivec4 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> glm::ivec4 { return lhs * rhs; },
        [](const glm::ivec4& lhs, int rhs) -> glm::ivec4 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> glm::ivec4 { return lhs / rhs; },
        [](const glm::ivec4& lhs, int rhs) -> glm::ivec4 { return lhs / rhs; }
      ),
      sol::meta_function::less_than, [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::ivec4& lhs, const glm::ivec4& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::ivec4& self) -> glm::ivec4 { return -self; },
      sol::meta_function::to_string, [](const glm::ivec4& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y) + "," + std::to_string(self.z) + "," + std::to_string(self.w); },
      "x", &glm::ivec4::x,
      "y", &glm::ivec4::y,
      "z", &glm::ivec4::z,
      "w", &glm::ivec4::w,
      "r", &glm::ivec4::r,
      "g", &glm::ivec4::g,
      "b", &glm::ivec4::b,
      "a", &glm::ivec4::a,
      "abs", [](const glm::ivec4& self) -> glm::ivec4 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::ivec4& self, const glm::ivec4& val) -> glm::ivec4 { return glm::min(self, val); },
        [](const glm::ivec4& self, int val) -> glm::ivec4 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::ivec4& self, const glm::ivec4& val) -> glm::ivec4 { return glm::max(self, val); },
        [](const glm::ivec4& self, int val) -> glm::ivec4 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::ivec4& self, const glm::ivec4& min, const glm::ivec4& max) -> glm::ivec4 { return glm::clamp(self, min, max); },
        [](const glm::ivec4& self, int min, int max) -> glm::ivec4 { return glm::clamp(self, min, max); }
      ));

    state.new_usertype<glm::vec4>(
      "Vec4",
      sol::meta_function::construct, sol::constructors<void(),
                                                       void(float), void(float, float, float, float),
                                                       void(const glm::vec2&, float, float), void(const glm::ivec2&, int, int),
                                                       void(const glm::vec2&, const glm::vec2&), void(const glm::ivec2&, const glm::ivec2&),
                                                       void(const glm::vec3&, float), void(const glm::ivec3&, int),
                                                       void(const glm::vec4&), void(const glm::ivec4&)>(),
      sol::call_constructor, sol::constructors<void(),
                                               void(float), void(float, float, float, float),
                                               void(const glm::vec2&, float, float), void(const glm::ivec2&, int, int),
                                               void(const glm::vec2&, const glm::vec2&), void(const glm::ivec2&, const glm::ivec2&),
                                               void(const glm::vec3&, float), void(const glm::ivec3&, int),
                                               void(const glm::vec4&), void(const glm::ivec4&)>(),
      sol::meta_function::type, state.create_table_with("name", "Vec4"),
      sol::meta_function::equal_to, [](const glm::vec4& lhs, const glm::vec4& rhs) -> bool { return lhs == rhs; },
      sol::meta_function::addition, sol::overload(
        [](const glm::vec4& lhs, const glm::vec4& rhs) -> glm::vec4 { return lhs + rhs; },
        [](const glm::vec4& lhs, float rhs) -> glm::vec4 { return lhs + rhs; }
      ),
      sol::meta_function::subtraction, sol::overload(
        [](const glm::vec4& lhs, const glm::vec4& rhs) -> glm::vec4 { return lhs - rhs; },
        [](const glm::vec4& lhs, float rhs) -> glm::vec4 { return lhs - rhs; }
      ),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec4& lhs, const glm::vec4& rhs) -> glm::vec4 { return lhs * rhs; },
        [](const glm::vec4& lhs, float rhs) -> glm::vec4 { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec4& lhs, const glm::vec4& rhs) -> glm::vec4 { return lhs / rhs; },
        [](const glm::vec4& lhs, float rhs) -> glm::vec4 { return lhs / rhs; }
      ),
      sol::meta_function::less_than, [](const glm::vec4& lhs, const glm::vec4& rhs) -> bool { return glm::all(glm::lessThan(lhs, rhs)); },
      sol::meta_function::less_than_or_equal_to, [](const glm::vec4& lhs, const glm::vec4& rhs) -> bool { return glm::all(glm::lessThanEqual(lhs, rhs)); },
      sol::meta_function::unary_minus, [](const glm::vec4& self) -> glm::vec4 { return -self; },
      sol::meta_function::to_string, [](const glm::vec4& self) -> std::string { return std::to_string(self.x) + "," + std::to_string(self.y) + "," + std::to_string(self.z) + "," + std::to_string(self.w); },
      "x", &glm::vec4::x,
      "y", &glm::vec4::y,
      "z", &glm::vec4::z,
      "w", &glm::vec4::w,
      "r", &glm::vec4::r,
      "g", &glm::vec4::g,
      "b", &glm::vec4::b,
      "a", &glm::vec4::a,
      "floor", [](const glm::vec4& self) -> glm::vec4 { return glm::floor(self); },
      "ceil", [](const glm::vec4& self) -> glm::vec4 { return glm::ceil(self); },
      "round", [](const glm::vec4& self) -> glm::vec4 { return glm::round(self); },
      "abs", [](const glm::vec4& self) -> glm::vec4 { return glm::abs(self); },
      "min", sol::overload(
        [](const glm::vec4& self, const glm::vec4& val) -> glm::vec4 { return glm::min(self, val); },
        [](const glm::vec4& self, float val) -> glm::vec4 { return glm::min(self, val); }
      ),
      "max", sol::overload(
        [](const glm::vec4& self, const glm::vec4& val) -> glm::vec4 { return glm::max(self, val); },
        [](const glm::vec4& self, float val) -> glm::vec4 { return glm::max(self, val); }
      ),
      "clamp", sol::overload(
        [](const glm::vec4& self, const glm::vec4& min, const glm::vec4& max) -> glm::vec4 { return glm::clamp(self, min, max); },
        [](const glm::vec4& self, float min, float max) -> glm::vec4 { return glm::clamp(self, min, max); }
      ));
  }
}

#endif // ! GLM_BINDINGS_H
