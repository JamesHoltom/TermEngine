#include <algorithm>

#include "ObjectManager.h"

namespace term_engine::objects {
  void Render()
  {
    if (Object::IsDirty()) {
      for (auto& glyph : glyphs::data) {
        glyph.Set(glyphs::default_glyph);
      }

      for (auto& object : objects::object_list) {
        object->Render(glyphs::data);
      }

      Object::SetDirty(false);
    }
  }

  void CleanUp()
  {
    object_list.clear();
  }

  ObjectPtr& Add(const glm::vec2& position, const glm::ivec2& size)
  {
    return object_list.emplace_back(std::make_shared<Object>(position, size));
  }

  void Remove(const ObjectPtr& obj)
  {
    ObjectList::iterator position = std::find(object_list.begin(), object_list.end(), obj);

    if (position != object_list.end()) {
      object_list.erase(position);
    }
  }

  ObjectList object_list;
}