/// @author James Holtom

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Object.h"
#include "../glyphs/Glyph.h"

namespace term_engine::objects {
  /// Smart pointer to an object.
  typedef std::shared_ptr<Object> ObjectPtr;
  /// Used to store a list of objects.
  typedef std::unordered_map<std::string, ObjectPtr> ObjectList;
  /// Represents the key-value pairs in the list of objects.
  typedef std::pair<std::string, ObjectPtr> ObjectPair;

  /// Renders the list of objects to the window.
  void Render();

  /// Destroys all objects in the list.
  void CleanUp();

  /// Adds an object to the list.
  /**
   * @param[in] name    The name of the object.
   * @param[in] postion The position of the object.
   * @param[in] size    The size of the object, in rows & columns.
   * @returns A smart pointer to the object if it was added to the list, or a null pointer if it failed.
   */
  ObjectPtr Add(const std::string& name, const glm::vec2& position, const glm::ivec2& size);

  /// Retrieves an object from the list.
  /**
   * @param[in] name The name of the object.
   * @returns A smart pointer to the object if it's in the list, or a null pointer.
   */
  ObjectPtr Get(const std::string& name);

  /// Removes an object from the list.
  /**
   * @param[in] name The name of the object.
   */
  void Remove(const std::string& name);

  /// The list of objects.
  extern ObjectList object_list;
}

#endif // ! OBJECT_MANAGER_H
