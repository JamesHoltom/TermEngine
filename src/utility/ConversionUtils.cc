#include "ConversionUtils.h"

namespace term_engine::utility {
  uint32_t GetIndexFromPosition(usertypes::GameObject* game_object, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(GetRowColFromPosition(game_object, position));

    return GetIndexFromRowCol(game_object, rowcol);
  }

  uint32_t GetIndexFromPosition(usertypes::GameScene* game_scene, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(GetRowColFromPosition(game_scene, position));

    return GetIndexFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(usertypes::GameObject* game_object, uint32_t index)
  {
    glm::ivec2 rowcol(GetRowColFromIndex(game_object, index));

    return GetPositionFromRowCol(game_object, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(usertypes::GameScene* game_scene, uint32_t index)
  {
    glm::ivec2 rowcol(GetRowColFromIndex(game_scene, index));

    return GetPositionFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(usertypes::GameObject* game_object, const glm::ivec2& position)
  {
    usertypes::GameScene* game_scene = game_object->GetGameScene();
    glm::ivec2 rowcol(position / game_scene->GetFont()->GetCharacterSize(game_scene->GetFontSize()));

    return rowcol;
  }

  glm::ivec2 GetRowColFromPosition(usertypes::GameScene* game_scene, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(position / game_scene->GetFont()->GetCharacterSize(game_scene->GetFontSize()));

    return rowcol;
  }

  glm::ivec2 GetPositionFromRowCol(usertypes::GameObject* game_object, const glm::ivec2& rowcol)
  {
    usertypes::GameScene* game_scene = game_object->GetGameScene();
    glm::ivec2 position(rowcol * game_scene->GetFont()->GetCharacterSize(game_scene->GetFontSize()));

    return position;
  }

  glm::ivec2 GetPositionFromRowCol(usertypes::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    uint32_t font_size = game_scene->GetFontSize();
    glm::ivec2 position(rowcol * game_scene->GetFont()->GetCharacterSize(font_size));

    return position;
  }

  glm::ivec2 GetRowColFromIndex(usertypes::GameObject* game_object, uint32_t index)
  {
    glm::ivec2 size(game_object->GetSize());

    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::GameScene* game_scene, uint32_t index)
  {
    glm::ivec2 size(game_scene->GetCharacterMap()->GetSize());
    
    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, uint32_t index)
  {
    return glm::ivec2(index % size.x, index / size.x);
  }

  uint32_t GetIndexFromRowCol(usertypes::GameObject* game_object, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_object->GetSize());

    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::GameScene* game_scene, const glm::ivec2& rowcol)
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
