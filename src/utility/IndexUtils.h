/// @author James Holtom

#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <glm/glm.hpp>
#include "../objects/GameObject.h"
#include "../objects/GameScene.h"
#include "../rendering/CharacterMap.h"
#include "../rendering/FontAtlas.h"

namespace term_engine::utility {
  /**
   * @brief Returns the data index for the given position, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the data index from.
   * @param[in] position    The position to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromPosition(objects::GameObject* game_object, const glm::ivec2& position);

  /**
   * @brief Returns the data index for the given position, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the data index from.
   * @param[in] position    The position to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromPosition(objects::GameScene* game_scene, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given data index, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the position from.
   * @param[in] index       The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(objects::GameObject* game_object, uint32_t index);

  /**
   * @brief Returns the position for the given data index, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the position from.
   * @param[in] index       The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(objects::GameScene* game_scene, uint32_t index);

  /**
   * @brief Returns the row/column for the given position, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the row/column from.
   * @param[in] position    The position to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromPosition(objects::GameObject* game_object, const glm::ivec2& position);

  /**
   * @brief Returns the row/column for the given position, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the row/column from.
   * @param[in] position    The position to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromPosition(objects::GameScene* game_scene, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given row/column, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the position from.
   * @param[in] rowcol      The row/column to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol);

  /**
   * @brief Returns the position for the given row/column, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the position from.
   * @param[in] rowcol      The row/column to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol);

  /**
   * @brief Returns the row/column for the given data index, based on the game scene passed.
   * 
   * @param[in] game_object The game object to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(objects::GameObject* game_object, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(objects::GameScene* game_scene, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index.
   * @note This is an overload for internal use within the CharacterMap class.
   * 
   * @param[in] character_map The character map to derive the row/column from.
   * @param[in] index         The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, uint32_t index);

  /**
   * @brief Returns the data index for the given row/column, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column.
   * @note This is an overload for internal use within the CharacterMap class.
   * 
   * @param[in] character_map The character map to derive the data index from.
   * @param[in] rowcol        The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol);

  /**
   * @brief Converts a C-style index (i.e. starting from 0) into a Lua-style index (i.e. starting from 1).
   * 
   * @param[in] index The index to convert.
   * @returns The converted index.
   */
  uint32_t ToLuaIndex(uint32_t index);

  /**
   * @brief Converts a Lua-style index (i.e. starting from 1) into a C-style index (i.e. starting from 0).
   * 
   * @param[in] index The index to convert.
   * @returns The converted index.
   */
  uint32_t ToCppIndex(uint32_t index);
}

#endif // ! INDEX_UTILS_H
