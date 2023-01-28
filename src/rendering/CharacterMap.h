/// @author James Holtom

#ifndef CHARACTER_MAP_H
#define CHARACTER_MAP_H

#include "Buffer.h"
#include "Character.h"
#include "FontAtlas.h"
#include "../utility/GLUtils.h"

namespace term_engine::rendering {
  /// @brief The default number of rows/columns in the view.
  constexpr glm::ivec2 DEFAULT_CHARACTER_MAP_SIZE = glm::ivec2(32, 16);

  class CharacterMap {
  public:
    /// @brief Constructs the character map.
    CharacterMap();

    /**
     * @brief Gets the position of the view on the window.
     * 
     * @returns The position of the view.
     */
    glm::vec2 GetPosition();

    /**
     * @brief Gets the size of the view.
     * 
     * @returns The size of the view.
     */
    glm::ivec2 GetSize();

    /**
     * @brief Sets the position of the view on the window.
     * 
     * @param[in] position The position to move the view to.
     */
    void SetPosition(const glm::vec2& position);

    /**
     * @brief Sets the size of the view, in rows/columns.
     * 
     * @param[in] size The new size of the view.
     */
    void SetSize(const glm::ivec2& size);

    /**
     * @brief Pushes character data to the buffer at the given index.
     * 
     * @param[in] data The character data to push.
     */
    void PushCharacters(const glm::ivec2& position, const glm::ivec2& size, const CharacterData& data);

    /// @brief Renders the list of views to the window.
    void CopyToBuffer(Buffer& buffer, FontAtlas& font_atlas) const;

  private:
    /// @brief The position of the view.
    glm::vec2 position_;
    /// @brief The size of the view, in rows/columns.
    glm::ivec2 size_;
    /// @brief The character data to be rendered to a window.
    CharacterData data_;
  };
}

#endif // ! CHARACTER_MAP_H
