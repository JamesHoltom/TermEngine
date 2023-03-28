/// @author James Holtom

#ifndef INDEX_FUNCTIONS_H
#define INDEX_FUNCTIONS_H

#include "../objects/GameObject.h"
#include "../objects/GameScene.h"

namespace term_engine::scripting {
  
  /**
   * @brief Returns the data index for the given position, based on the game object passed.
   * @note This function returns the index in Lua-style.
   * 
   * @param[in] game_object The game object to derive the data index from.
   * @param[in] position    The position to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromPosition(objects::GameObject* game_object, const glm::ivec2& position);

  /**
   * @brief Returns the data index for the given position, based on the game scene passed.
   * @note This function returns the index in Lua-style.
   * 
   * @param[in] game_scene  The game scene to derive the data index from.
   * @param[in] position    The position to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromPosition(objects::GameScene* game_scene, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given data index, based on the game object passed.
   * @note This function converts the index to C++-style.
   * 
   * @param[in] game_object The game object to derive the position from.
   * @param[in] index       The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(objects::GameObject* game_object, uint32_t index);

  /**
   * @brief Returns the position for the given data index, based on the game scene passed.
   * @note This function converts the index to C++-style.
   * 
   * @param[in] game_scene  The game scene to derive the position from.
   * @param[in] index       The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(objects::GameScene* game_scene, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game scene passed.
   * @note This function converts the index to C++-style.
   * 
   * @param[in] game_object The game object to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(objects::GameObject* game_object, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game scene passed.
   * @note This function converts the index to C++-style.
   * 
   * @param[in] game_scene  The game scene to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(objects::GameScene* game_scene, uint32_t index);

  /**
   * @brief Returns the data index for the given row/column, based on the game object passed.
   * @note This function returns the index in Lua-style.
   * 
   * @param[in] game_object The game object to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game scene passed.
   * @note This function returns the index in Lua-style.
   * 
   * @param[in] game_scene  The game scene to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol);

}

#endif // ! INDEX_FUNCTIONS_H
