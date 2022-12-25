#include "IndexUtils.h"
#include "../resources/FontAtlas.h"

namespace term_engine {
  int GetIndexFromPosition(const glm::ivec2& size, const glm::ivec2& position)
  {
    glm::ivec2 rowcol = GetRowColFromPosition(position);

    return GetIndexFromRowCol(size, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(const glm::ivec2& position)
  {
    return position / fonts::character_size;
  }

  glm::ivec2 GetPositionFromIndex(const glm::ivec2& size, const int& index)
  {
    glm::ivec2 rowcol = GetRowColFromIndex(size, index);

    return GetPositionFromRowCol(rowcol);
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, const int& index)
  {
    int cIndex = index - 1;
    return glm::ivec2(cIndex / size.x, cIndex % size.x);
  }

  int GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol)
  {
    return (rowcol.y * size.x) + rowcol.x + 1;
  }

  glm::ivec2 GetPositionFromRowCol(const glm::ivec2& rowcol)
  {
    return rowcol * fonts::character_size;
  }
}
