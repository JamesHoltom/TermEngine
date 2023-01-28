#include "IndexUtils.h"

namespace term_engine::utility {
  int GetIndexFromPosition(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& size, const glm::ivec2& position)
  {
    glm::ivec2 rowcol = GetRowColFromPosition(game_scene, position);

    return GetIndexFromRowCol(size, rowcol);
  }

  int GetIndexFromPosition(const rendering::FontAtlas& font_atlas, const glm::ivec2& size, const glm::ivec2& position)
  {
    glm::ivec2 rowcol = GetRowColFromPosition(font_atlas, position);

    return GetIndexFromRowCol(size, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& size, const int& index)
  {
    glm::ivec2 rowcol = GetRowColFromIndex(size, index);

    return GetPositionFromRowCol(game_scene, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(const rendering::FontAtlas& font_atlas, const glm::ivec2& size, const int& index)
  {
    glm::ivec2 rowcol = GetRowColFromIndex(size, index);

    return GetPositionFromRowCol(font_atlas, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& position)
  {
    objects::GameScenePtr ptr = game_scene.lock();
    glm::ivec2 rowcol(position / ptr->GetFontAtlas().GetCharacterSize());

    return rowcol;
  }

  glm::ivec2 GetRowColFromPosition(const rendering::FontAtlas& font_atlas, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(position / font_atlas.GetCharacterSize());

    return rowcol;
  }

  glm::ivec2 GetPositionFromRowCol(const objects::GameSceneWeakPtr& game_scene, const glm::ivec2& rowcol)
  {
    objects::GameScenePtr ptr = game_scene.lock();
    glm::ivec2 position(rowcol * ptr->GetFontAtlas().GetCharacterSize());

    return position;
  }

  glm::ivec2 GetPositionFromRowCol(const rendering::FontAtlas& font_atlas, const glm::ivec2& rowcol)
  {
    glm::ivec2 position(rowcol * font_atlas.GetCharacterSize());

    return position;
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, const int& index)
  {
    return glm::ivec2(index % size.x, index / size.x);
  }

  int GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol)
  {
    return (rowcol.y * size.x) + rowcol.x;
  }

  int ToLuaIndex(const int& index)
  {
    return index + 1;
  }

  int ToCppIndex(const int& index)
  {
    return index - 1;
  }
}
