#include "BaseObject.h"

namespace term_engine::objects {
  glm::vec2 BaseObject::GetPosition() const
  {
    return position_;
  }

  void BaseObject::SetPosition(const glm::vec2& position)
  {
    position_ = position;
    is_dirty = true;
  }

  void Render()
  {
    if (is_dirty) {
      for (auto& glyph : glyphs::data) {
        glyph.Set(glyphs::default_glyph);
      }

      for (auto& object : objects::object_list) {
        object->Render(glyphs::data);
      }

      is_dirty = false;
    }
  }

  void CleanUp()
  {
    for (auto& object : objects::object_list) {
      delete object;
      object = nullptr;
    }

    object_list.clear();
  }

  bool is_dirty = true;
  ObjectList object_list;
}
