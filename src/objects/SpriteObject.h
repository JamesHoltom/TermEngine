/// @author James Holtom

#ifndef SPRITE_OBJECT_H
#define SPRITE_OBJECT_H

#include "BaseObject.h"

namespace term_engine::objects {
  class SpriteObject : public BaseObject
  {
  public:
    /// Renders the sprite to the given glyph set.
    /**
      * @param[in] data
      */
    void Render(const glyphs::BufferList& data);

  private:
  };
}

#endif // ! SPRITE_OBJECT_H
