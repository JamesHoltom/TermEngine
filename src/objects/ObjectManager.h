/// @author James Holtom

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "Object.h"

namespace term_engine::objects {
  /// Smart pointer to an object.
  typedef std::shared_ptr<Object> ObjectPtr;
  /// Used to store a list of objects.
  typedef std::vector<ObjectPtr> ObjectList;

  /// The list of objects.
  extern ObjectList object_list;

  /// Destroys all objects in the list.
  void CleanUp();

  /// Adds an object to the list.
  /**
   * @param[in] position The position of the object.
   * @param[in] size     The size of the object, in rows & columns.
   * @returns A smart pointer to the object if it was added to the list, or a null pointer if it failed.
   */
  ObjectPtr& Add(const glm::vec2& position, const glm::ivec2& size);

  /// Removes an object from the list.
  /**
   * @param[in] obj A smart pointer to the object.
   */
  void Remove(const ObjectPtr& obj);
}

#endif // ! OBJECT_MANAGER_H
