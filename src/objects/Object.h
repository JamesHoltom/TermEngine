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

  /// Used to store a collection of glyph parameters, which represents an object.
  typedef std::vector<glyphs::GlyphParams> GlyphData;

  /// 
  class Object {
  public:
    /// Constructs the object with the given parameters.
    /**
     * @param[in] name     The name of the object to add. This can later be retrieved in-script with "objects.get()".
     * @param[in] position The position of the object in the window.
     * @param[in] size     The size of the object, i.e. how many rows & columns.
     */
    Object(const std::string& name, const glm::vec2& position, const glm::ivec2& size);

    /// Renders the object to the given buffer.
    /**
     * @param[in] data The data to render.
     */
    void Render(glyphs::BufferList& data) const;

    /// Returns the name of the object.
    /**
     * @returns The name of the object.
     */
    std::string GetName() const;

    /// Returns the position of the object.
    /**
     * @returns The position of the object.
     */
    glm::vec2 GetPosition() const;

    /// Returns the size of the object.
    /**
     * @returns The size of the object.
     */
    glm::ivec2 GetSize() const;

    /// Returns a reference to the object's data.
    /**
     * @returns The data within the object.
     */
    GlyphData& GetData();

    /// Sets the position of the object.
    /**
     * @param[in] position The position to move the object to.
     */
    void SetPosition(const glm::vec2& position);

    /// Sets the size of the object.
    /**
     * @param[in] size The new size of the object.
     */
    void SetSize(const glm::ivec2& size);

  protected:
    /// The name of the object.
    std::string name_;
    /// The top-left position of the object.
    glm::vec2 position_;
    /// The size of the object, in rows & columns.
    glm::ivec2 size_;
    /// The glyph parameters that will copied to the buffer when rendered.
    GlyphData data_;
  };

  /// A flag to check if any objects have been modified, and will need to be re-rendered.
  extern bool is_dirty;
}

#endif // ! OBJECT_H
