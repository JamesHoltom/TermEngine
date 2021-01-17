/// @author James Holtom

#ifndef LOGGING_UTILS_H
#define LOGGING_UTILS_H

#include <ostream>
#include <glm/glm.hpp>

/// Allows _std::stringstream_ to correctly parse a _glm::vec2_ object.
/**
  * @param[in] os The stream to output the value to.
  * @param[in] value The value to parse.
  * @returns The updated stream.
  */
std::ostream& operator<< (std::ostream& os, const glm::vec2& value);

/// Allows _std::stringstream_ to correctly parse a _glm::vec4_ object.
/**
  * @param[in] os The stream to output the value to.
  * @param[in] value The value to parse.
  * @returns The updated stream.
  */
std::ostream& operator<< (std::ostream& os, const glm::vec4& value);

/// Allows _std::stringstream_ to correctly parse a _glm::uvec2_ object.
/**
  * @param[in] os The stream to output the value to.
  * @param[in] value The value to parse.
  * @returns The updated stream.
  */
std::ostream& operator<< (std::ostream& os, const glm::uvec2& value);

/// Allows _std::stringstream_ to correctly parse a _glm::ivec2_ object.
/**
  * @param[in] os The stream to output the value to.
  * @param[in] value The value to parse.
  * @returns The updated stream.
  */
std::ostream& operator<< (std::ostream& os, const glm::ivec2& value);

#endif
