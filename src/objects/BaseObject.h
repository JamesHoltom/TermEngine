/// @author James Holtom

#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <list>
#include <memory>
#include <glm/glm.hpp>

#include "../glyphs/Glyph.h"

namespace term_engine::objects {
  class BaseObject;

  typedef std::list<BaseObject*> ObjectList;

  class BaseObject {
  public:
    /// Destroys the object.
    virtual ~BaseObject() {};

    /// Renders the object to the given glyph set.
    /**
     * @param[in] data
     */
    virtual void Render(glyphs::BufferList& data) = 0;

    /// Returns the position of the object.
    /**
     * @returns The position of the object.
     */
    glm::ivec2 GetPosition() const;

    /// Sets the position of the object.
    /**
     * @param[in] position The position to move the object to.
     */
    void SetPosition(const glm::ivec2& position);

  protected:
    /// The top-left position of the object, within a glyph set.
    glm::ivec2 position_;
  };

  void Render();
  void CleanUp();

  extern bool is_dirty;
  extern ObjectList object_list;
}

#endif // ! BASE_OBJECT_H
