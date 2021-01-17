#include "LoggingUtils.h"

std::ostream& operator<< (std::ostream& os, const glm::vec2& value)
{
  return os << value.x << ", " << value.y;
}

std::ostream& operator<< (std::ostream& os, const glm::vec4& value)
{
  return os << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
}

std::ostream& operator<< (std::ostream& os, const glm::uvec2& value)
{
  return os << value.x << ", " << value.y;
}

std::ostream& operator<< (std::ostream& os, const glm::ivec2& value)
{
  return os << value.x << ", " << value.y;
}
