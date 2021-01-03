/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef LOGGING_UTILS_H
#define LOGGING_UTILS_H

#include <ostream>
#include <glm/glm.hpp>

std::ostream& operator<< (std::ostream& os, const glm::vec2& value);
std::ostream& operator<< (std::ostream& os, const glm::vec4& value);
std::ostream& operator<< (std::ostream& os, const glm::uvec2& value);
std::ostream& operator<< (std::ostream& os, const glm::ivec2& value);

#endif
