/// @author James Holtom

#ifndef INDEX_UTILS_H
#define INDEX_UTILS_H

#include <glm/glm.hpp>
#include "../usertypes/GameScene.h"
#include "../usertypes/GameWindow.h"
#include "../usertypes/game_objects/GameObject.h"
#include "../usertypes/resources/Animation.h"

namespace term_engine::utility {
  /**
   * @brief Returns the data index for the given position, based on the game window passed.
   * 
   * @param[in] game_window Raw pointer to the game window to derive the data index from.
   * @param[in] position    The position to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromPosition(usertypes::GameWindow* game_window, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given data index, based on the game window passed.
   * 
   * @param[in] game_window The game window to derive the position from.
   * @param[in] index       The data index to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromIndex(usertypes::GameWindow* game_window, uint32_t index);

  /**
   * @brief Returns the row/column for the given position, based on the game window passed.
   * 
   * @param[in] game_window The game window to derive the row/column from.
   * @param[in] position    The position to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromPosition(usertypes::GameWindow* game_window, const glm::ivec2& position);

  /**
   * @brief Returns the position for the given row/column, based on the game window passed.
   * 
   * @param[in] game_window The game window to derive the position from.
   * @param[in] rowcol      The row/column to retrieve the position for.
   * @returns The position.
   */
  glm::ivec2 GetPositionFromRowCol(usertypes::GameWindow* game_window, const glm::ivec2& rowcol);

  /**
   * @brief Returns the row/column for the given data index, based on a generic size parameter.
   * 
   * @param[in] size  The size of the container to derive the row/column from.
   * @param[in] index The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the character map passed.
   * 
   * @param[in] character_map The character map to derive the row/column from.
   * @param[in] index         The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(usertypes::CharacterMap* character_map, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(usertypes::GameObject* game_object, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game object passed.
   * 
   * @param[in] anim_frame  The animation frame to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(usertypes::AnimationFrame* anim_frame, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game scene passed.
   * 
   * @param[in] game_scene  Raw pointer to the game scene to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(usertypes::GameScene* game_scene, uint32_t index);

  /**
   * @brief Returns the row/column for the given data index, based on the game window passed.
   * 
   * @param[in] game_window Raw pointer to the game window to derive the row/column from.
   * @param[in] index       The data index to retrieve the row/column for.
   * @returns The row/column.
   */
  glm::ivec2 GetRowColFromIndex(usertypes::GameWindow* game_window, uint32_t index);

  /**
   * @brief Returns the data index for the given row/column, based on a generic size parameter.
   * @note This is an overload for internal use within the CharacterMap class.
   * 
   * @param[in] size    The size of the container to derive the data index from.
   * @param[in] rowcol  The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the character map passed.
   * 
   * @param[in] character_map The character map to derive the data index from.
   * @param[in] rowcol        The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(usertypes::CharacterMap* character_map, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game object passed.
   * 
   * @param[in] game_object The game object to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(usertypes::GameObject* game_object, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game object passed.
   * 
   * @param[in] anim_frame  The animation frame to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(usertypes::AnimationFrame* anim_frame, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game scene passed.
   * 
   * @param[in] game_scene  The game scene to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(usertypes::GameScene* game_scene, const glm::ivec2& rowcol);

  /**
   * @brief Returns the data index for the given row/column, based on the game window passed.
   * 
   * @param[in] game_window The game window to derive the data index from.
   * @param[in] rowcol      The row/column to retrieve the data index for.
   * @returns The data index.
   */
  uint32_t GetIndexFromRowCol(usertypes::GameWindow* game_window, const glm::ivec2& rowcol);

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
