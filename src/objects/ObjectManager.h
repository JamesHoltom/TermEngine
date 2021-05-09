/// @author James Holtom

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Object.h"
#include "../glyphs/Glyph.h"

namespace term_engine::objects {
  typedef std::shared_ptr<Object> ObjectPtr;
  typedef std::unordered_map<std::string, ObjectPtr> ObjectList;
  typedef std::pair<std::string, ObjectPtr> ObjectPair;

  void Render();
  void CleanUp();

  ObjectPtr Add(const std::string& name, const glm::vec2& position, const glm::ivec2& size);
  ObjectPtr Get(const std::string& name);
  void Remove(const std::string& name);

  extern ObjectList object_list;
}

#endif // ! OBJECT_MANAGER_H
