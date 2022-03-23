/// @author James Holtom

#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../utility/Glyph.h"

namespace term_engine::objects {
  /// Used to store a collection of glyph parameters, which represents an object.
  typedef std::vector<GlyphParams> GlyphData;

  /// Used to represent a game object, that is rendered to the screen.
  /**
   * Other attributes can be added at the script level; this structure only cares about rendering glyphs to the screen.
   */
  class Object {
  public:
    /// Constructs the object with the given parameters.
    /**
     * @param[in] position The position of the object in the window.
     * @param[in] size     The size of the object, i.e. how many rows & columns.
     */
    Object(const glm::vec2& position, const glm::ivec2& size);

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

    /// Returns if the object is active.
    /**
     * @returns If the object is active.
     */
    bool IsActive() const;

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

    /// Sets if the object is active.
    /**
     * @param[in] flag The value to set.
     */
    void SetActive(const bool& flag);

    /// Returns if 2 objects share the same ID.
    /**
     * @param[in] rhs The object to compare this object to.
     * @returns If the 2 objects are the same.
     */
    bool operator==(const Object& rhs) const
    {
      return object_id_ == rhs.object_id_;
    }

    /// Returns if the 'Is Dirty' flag is set, and there are objects that need to be re-rendered.
    /**
     * @returns If the 'Is Dirty' flag is set.
     */
    static bool IsDirty();

    /// Sets the 'Is Dirty' flag to true.
    static void SetDirty();

    /// Sets the 'Is Dirty' flag to false;
    static void Clean();

  protected:
    /// The top-left position of the object.
    glm::vec2 position_;
    /// The size of the object, in rows & columns.
    glm::ivec2 size_;
    /// The glyph parameters that will copied to the buffer when rendered.
    GlyphData data_;
    /// The ID of the object.
    int object_id_;
    /// Is the object active? (i.e. Is the object being rendered and acted on?)
    bool is_active_;

    /// Represents the ID to use for the next object.
    static int object_next_id_;
    /// A flag to check if any objects have been modified, and will need to be re-rendered.
    static bool is_dirty_;
  };
}

#endif // ! OBJECT_H
