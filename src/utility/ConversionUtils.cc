#include "ConversionUtils.h"

namespace term_engine::utility {
  uint32_t GetIndexFromPosition(usertypes::GameWindow* game_window, const glm::ivec2& position)
  {
    glm::ivec2 rowcol(GetRowColFromPosition(game_window, position));

    return GetIndexFromRowCol(game_window, rowcol);
  }

  glm::ivec2 GetPositionFromIndex(usertypes::GameWindow* game_window, uint32_t index)
  {
    glm::ivec2 rowcol(GetRowColFromIndex(game_window, index));

    return GetPositionFromRowCol(game_window, rowcol);
  }

  glm::ivec2 GetRowColFromPosition(usertypes::GameWindow* game_window, const glm::ivec2& position)
  {
    uint32_t font_size = game_window->GetFontSize();
    glm::ivec2 rowcol(position / game_window->GetFont()->GetCharacterSize(font_size));

    return rowcol;
  }

  glm::ivec2 GetPositionFromRowCol(usertypes::GameWindow* game_window, const glm::ivec2& rowcol)
  {
    uint32_t font_size = game_window->GetFontSize();
    glm::ivec2 position(rowcol * game_window->GetFont()->GetCharacterSize(font_size));

    return position;
  }

  glm::ivec2 GetRowColFromIndex(const glm::ivec2& size, uint32_t index)
  {
    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::CharacterMap* character_map, uint32_t index)
  {
    glm::ivec2 size(character_map->GetSize());

    return GetRowColFromIndex(size, index);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::GameObject* game_object, uint32_t index)
  {
    glm::ivec2 size(game_object->GetCharacterMap().GetSize());

    return GetRowColFromIndex(size, index);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::AnimationFrame* anim_frame, uint32_t index)
  {
    glm::ivec2 size(anim_frame->GetCharacterMap().GetSize());

    return GetRowColFromIndex(size, index);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::GameScene* game_scene, uint32_t index)
  {
    glm::ivec2 size(game_scene->GetCharacterMap()->GetSize());
    
    return glm::ivec2(index % size.x, index / size.x);
  }

  glm::ivec2 GetRowColFromIndex(usertypes::GameWindow* game_window, uint32_t index)
  {
    glm::ivec2 size(game_window->GetGameScene()->GetCharacterMap()->GetSize());
    
    return glm::ivec2(index % size.x, index / size.x);
  }

  uint32_t GetIndexFromRowCol(const glm::ivec2& size, const glm::ivec2& rowcol)
  {
    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::CharacterMap* character_map, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(character_map->GetSize());

    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::GameObject* game_object, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_object->GetCharacterMap().GetSize());

    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::AnimationFrame* anim_frame, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(anim_frame->GetCharacterMap().GetSize());

    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::GameScene* game_scene, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_scene->GetCharacterMap()->GetSize());
    
    return (rowcol.y * size.x) + rowcol.x;
  }

  uint32_t GetIndexFromRowCol(usertypes::GameWindow* game_window, const glm::ivec2& rowcol)
  {
    glm::ivec2 size(game_window->GetGameScene()->GetCharacterMap()->GetSize());
    
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
