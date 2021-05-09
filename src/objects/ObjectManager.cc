#include "ObjectManager.h"

namespace term_engine::objects {
  void Render()
  {
    if (is_dirty) {
      for (auto& glyph : glyphs::data) {
        glyph.Set(glyphs::default_glyph);
      }

      for (auto& object : objects::object_list) {
        object.second->Render(glyphs::data);
      }

      is_dirty = false;
    }
  }

  void CleanUp()
  {
    object_list.clear();
  }

  ObjectPtr Add(const std::string& name, const glm::vec2& position, const glm::ivec2& size)
  {
    auto result = object_list.insert_or_assign(name, std::make_shared<Object>(name, position, size));

    return result.first->second;
  }

  ObjectPtr Get(const std::string& name)
  {
    auto result = object_list.find(name);

    if (result != object_list.end()) {
      return result->second;
    }
    else {
      return nullptr;
    }
  }

  void Remove(const std::string& name)
  {
    object_list.erase(name);
  }

  ObjectList object_list;
}