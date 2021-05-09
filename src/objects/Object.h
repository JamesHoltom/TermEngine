/// @author James Holtom

#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../glyphs/Glyph.h"

namespace term_engine::objects {
  class Object;

  typedef std::vector<glyphs::GlyphParams> GlyphData;

  class Object {
  public:
    Object(const std::string& name, const glm::vec2& position, const glm::ivec2& size);

    /// Renders the object to the given glyph set.
    /**
     * @param[in] data
     */
    void Render(glyphs::BufferList& data) const;

    std::string GetName() const;

    /// Returns the position of the object.
    /**
     * @returns The position of the object.
     */
    glm::vec2 GetPosition() const;

    glm::ivec2 GetSize() const;

    GlyphData& GetData();

    /// Sets the position of the object.
    /**
     * @param[in] position The position to move the object to.
     */
    void SetPosition(const glm::vec2& position);

    void SetSize(const glm::ivec2& size);

  protected:
    std::string name_;

    /// The top-left position of the object, within a glyph set.
    glm::vec2 position_;
    glm::ivec2 size_;

    GlyphData data_;
  };

  extern bool is_dirty;
}

#endif // ! OBJECT_H
