/// @author James Holtom

#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <glm/glm.hpp>

namespace term_engine {
  /**
   * @brief Returns the data index for the given position.
   * 
   * @param[in] size   The size of the object.
   * @param[in] position The position to retrieve the index for.
   * @returns The data index.
   */
  int GetIndexFromPosition(const glm::ivec2& size, const glm::ivec2& position);

  /**
   * @brief Returns the row/column for the given position.
   * 
   * @param[in] position The position to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromPosition(const glm::ivec2& position);

  /**
   * @brief Returns the position for the given data index.
   * @note This translates from Lua's index (starting from 1) to C++'s index (starting from 0).
   * 
   * @param[in] size   The size of the object.
   * @param[in] index The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(const glm::ivec2& size, const int& index);

  /**
   * @brief Returns the row/column for the given data index.
   * @note This translates from Lua's index (starting from 1) to C++'s index (starting from 0).
   * 
   * @param[in] size   The size of the object.
   * @param[in] index The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, const int& index);

  /**
   * @brief Returns the data index for the given row/column.
   * 
   * @param[in] size   The size of the object.
   * @param[in] rowcol The row/column to retrieve the index for.
   * @returns The data index.
   */
  int GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol);

  /**
   * @brief Returns the position for the given row/column.
   * 
   * @param[in] rowcol The row/column to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromRowCol(const glm::ivec2& rowcol);
}

#endif // ! INDEX_UTILS_H