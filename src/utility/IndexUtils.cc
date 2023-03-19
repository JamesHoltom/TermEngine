#include "IndexUtils.h"

namespace term_engine::utility {
  int GetIndexFromPosition(objects::GameScene* game_scene, const glm::ivec2& size, const glm::ivec2& position)
  {
    glm::ivec2 rowcol = GetRowColFromPosition(game_scene, position);

    return GetIndexFromRowCol(size, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(objects::GameScene* game_scene, const glm::ivec2& size, int index)
  {
    glm::ivec2 rowcol = GetRowColFromIndex(size, index);

    return GetPositionFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(objects::GameScene* game_scene, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(position / game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return rowcol;
  }

  glm::ivec2 GetPositionFromRowCol(objects::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    glm::ivec2 position(rowcol * game_scene->GetFontAtlas()->GetCharacterSize(game_scene->GetFontSize()));

    return position;
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, int index)
  {
    return glm::ivec2(index % size.x, index / size.x);
  }

  int GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol)
  {
    return (rowcol.y * size.x) + rowcol.x;
  }

  int ToLuaIndex(int index)
  {
    return index + 1;
  }

  int ToCppIndex(int index)
  {
    return index - 1;
  }
}
