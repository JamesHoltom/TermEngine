#include "IndexFunctions.h"
#include "../utility/IndexUtils.h"

namespace term_engine::scripting {
  uint32_t GetIndexFromPosition(objects::GameObject* game_object, const glm::ivec2& position)
  {
    return utility::ToLuaIndex(utility::GetIndexFromPosition(game_object, position));
  }

  uint32_t GetIndexFromPosition(objects::GameScene* game_scene, const glm::ivec2& position)
  {
    return utility::ToLuaIndex(utility::GetIndexFromPosition(game_scene, position));
  }

  glm::ivec2 GetPositionFromIndex(objects::GameObject* game_object, uint32_t index)
  {
    return utility::GetPositionFromIndex(game_object, utility::ToCppIndex(index));
  }

  glm::ivec2 GetPositionFromIndex(objects::GameScene* game_scene, uint32_t index)
  {
    return utility::GetPositionFromIndex(game_scene, utility::ToCppIndex(index));
  }

  glm::ivec2 GetRowColFromIndex(objects::GameObject* game_object, uint32_t index)
  {
    return utility::GetRowColFromIndex(game_object, utility::ToCppIndex(index));
  }

  glm::ivec2 GetRowColFromIndex(objects::GameScene* game_scene, uint32_t index)
  {
    return utility::GetRowColFromIndex(game_scene, utility::ToCppIndex(index));
  }

  uint32_t GetIndexFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol)
  {
    return utility::ToLuaIndex(utility::GetIndexFromRowCol(game_object, rowcol));
  }

  uint32_t GetIndexFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    return utility::ToLuaIndex(utility::GetIndexFromRowCol(game_scene, rowcol));
  }
}