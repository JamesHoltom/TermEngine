/// @author James Holtom

#ifndef COLOR_H
#define COLOR_H

#include <ostream>

#include <glm/glm.hpp>

namespace term_engine {
  /// Represents an RGBA color.
  struct Color {
    /// The red component value.
    float r_;
    /// The green component value.
    float g_;
    /// The blue component value.
    float b_;
    /// The alpha component value.
    float a_;

    /// Constructs an empty _Color_ object.
    Color() : r_(0.0f), g_(0.0f), b_(0.0f), a_(255.0f) {}
    /// Constructs a _Color_ object with the given parameters.
    /**
     * @param[in] r The red component value.
     * @param[in] g The green component value.
     * @param[in] b The blue component value.
     * @param[in] a The alpha component value.
     */
    Color(const float& r, const float& g, const float& b, const float& a) : r_(r), g_(g), b_(b), a_(a) {}
    /// Constructs a _Color_ object with the given _glm::vec4_ object.
    /**
     * @param[in] color The _glm::vec4_ object to derive the color from.
     */
    Color(const glm::vec4& color) : r_(color.r), g_(color.g), b_(color.b), a_(color.a) {}

    /// Converts the color into a _glm::vec4_ object.
    glm::vec4 toVector() const {
      return glm::vec4(r_, g_, b_, a_);
    }
  };

  /// Allows _std::stringstream_ to correctly parse a _Color_ object.
  /**
   * @param[in] os The stream to output the value to.
   * @param[in] color The color to parse.
   * @returns The updated stream.
   */
  std::ostream& operator<<(std::ostream& os, const Color& color) {
    return os << "{" << color.r_ << ", " << color.g_ << ", " << color.b_ << ", " << color.a_ << "}";
  }
}

#endif // !COLOR_H
