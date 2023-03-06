/// @author James Holtom

#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <glm/glm.hpp>
#include "../objects/GameScene.h"
#include "../rendering/CharacterMap.h"
#include "../rendering/FontAtlas.h"

namespace term_engine::utility {
  /**
   * @brief Returns the data index for the given position.
   * 
   * @param[in] game_scene  The game scene with the font to derive the row/column from.
   * @param[in] size        The size of the object.
   * @param[in] position    The position to retrieve the index for.
   * @returns The data index.
   */
  int GetIndexFromPosition(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& size, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given data index.
   * 
   * @param[in] game_scene The game scene with the font to derive the row/column from.
   * @param[in] size   The size of the object.
   * @param[in] index The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& size, const int& index);

  /**
   * @brief Returns the row/column for the given position.
   * 
   * @param[in] game_scene The game scene with the font to derive the row/column from.
   * @param[in] position The position to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromPosition(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given row/column.
   * 
   * @param[in] game_scene The game scene with the font to derive the row/column from.
   * @param[in] rowcol The row/column to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromRowCol(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& rowcol);

  /**
   * @brief Returns the row/column for the given data index.
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
   * @brief Converts a C-style index (i.e. starting from 0) into a Lua-style index (i.e. starting from 1).
   * 
   * @param[in] index The index to convert.
   * @returns The converted index.
   */
  int ToLuaIndex(const int& index);

  /**
   * @brief Converts a Lua-style index (i.e. starting from 1) into a C-style index (i.e. starting from 0).
   * 
   * @param[in] index The index to convert.
   * @returns The converted index.
   */
  int ToCppIndex(const int& index);
}

#endif // ! INDEX_UTILS_H
