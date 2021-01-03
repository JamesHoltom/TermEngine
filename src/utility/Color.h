#ifndef COLOR_H
#define COLOR_H

#include <ostream>

#include <glm/glm.hpp>

struct Color {
  float r_;
  float g_;
  float b_;
  float a_;

  Color() : r_(0.0f), g_(0.0f), b_(0.0f), a_(255.0f) {}
  Color(const float& r, const float& g, const float& b, const float& a) : r_(r), g_(g), b_(b), a_(a) {}
  Color(const glm::vec4& color) : r_(color.r), g_(color.g), b_(color.b), a_(color.a) {}

  glm::vec4 toVector() const {
    return glm::vec4(r_, g_, b_, a_);
  }
};

std::ostream& operator<<(std::ostream& os, const Color& color) {
  return os << "{" << color.r_ << ", " << color.g_ << ", " << color.b_ << ", " << color.a_ << "}";
}

#endif // !COLOR_H
