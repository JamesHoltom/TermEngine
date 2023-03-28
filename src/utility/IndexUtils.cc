#include "IndexUtils.h"

namespace term_engine::utility {
  uint32_t GetIndexFromPosition(objects::GameObject* game_object, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(GetRowColFromPosition(game_object, position));

    return GetIndexFromRowCol(game_object, rowcol);
  }

  uint32_t GetIndexFromPosition(objects::GameScene* game_scene, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(GetRowColFromPosition(game_scene, position));

    return GetIndexFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(objects::GameObject* game_object, uint32_t index)
  {
    glm::ivec2 rowcol(GetRowColFromIndex(game_object, index));

    return GetPositionFromRowCol(game_object, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(objects::GameScene* game_scene, uint32_t index)
  {
    glm::ivec2 rowcol(GetRowColFromIndex(game_scene, index));

    return GetPositionFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(objects::GameObject* game_object, const glm::ivec2& position)
  {
    objects::GameScene* game_scene = game_object->GetGameScene();
    glm::ivec2 rowcol(position / game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return rowcol;
  }

  glm::ivec2 GetRowColFromPosition(objects::GameScene* game_scene, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(position / game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return rowcol;
  }

  glm::ivec2 GetPositionFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol)
  {
    objects::GameScene* game_scene = game_object->GetGameScene();
    glm::ivec2 position(rowcol * game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return position;
  }

  glm::ivec2 GetPositionFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    glm::ivec2 position(rowcol * game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return position;
  }

  glm::ivec2 GetRowColFromIndex(objects::GameObject* game_object, uint32_t index)
  {
    glm::ivec2 size(game_object->GetSize());

    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(objects::GameScene* game_scene, uint32_t index)
  {
    glm::ivec2 size(game_scene->GetCharacterMap()->GetSize());
    
    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, uint32_t index)
  {
    return glm::ivec2(index % size.x, index / size.x);
  }

  uint32_t GetIndexFromRowCol(objects::GameObject* game_object, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_object->GetSize());

    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_scene->GetCharacterMap()->GetSize());
    
    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol)
  {
    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t ToLuaIndex(uint32_t index)
  {
    return index + 1;
  }

  uint32_t ToCppIndex(uint32_t index)
  {
    return index - 1;
  }
}
