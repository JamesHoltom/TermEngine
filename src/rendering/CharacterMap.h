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

  /// @brief Defines a map of characters to render to a game scene.
  class CharacterMap {
  public:
    /// @brief Constructs the character map.
    CharacterMap();

    /**
     * @brief Gets the position of the character map on the game scene.
     * 
     * @returns The position of the character map.
     */
    glm::vec2 GetPosition() const;

    /**
     * @brief Gets the size of the character map.
     * 
     * @returns The size of the character map.
     */
    glm::ivec2 GetSize() const;

    /**
     * @brief Returns if empty characters, when rendering, should have a background colour set.
     * 
     * @returns If a background colour is set.
     */
    bool ShouldEmptyCharactersHaveBackground() const;

    /**
     * @brief Sets the position of the character map on the game scene.
     * 
     * @param[in] position The position to move the character map to.
     */
    void SetPosition(const glm::vec2& position);

    /**
     * @brief Sets the size of the character map, in rows/columns.
     * 
     * @param[in] size The new size of the character map.
     */
    void SetSize(const glm::ivec2& size);

    /**
     * @brief Sets if empty characters should have a background colour when rendering.
     * 
     * @param[in] flag If empty characters should have a background colour.
     */
    void SetEmptyCharacterBackground(bool flag);

    /// @brief Clears the character map.
    void Clear();

    /**
     * @brief Pushes character data to the buffer at the given index.
     * 
     * @param[in] data The character data to push.
     */
    void PushCharacters(const glm::ivec2& position, const glm::ivec2& size, const CharacterData& data);

    /**
     * @brief Copies the data from a character map into an OpenGL buffer.
     * 
     * @param[in] character_map 
     * @param[in,out] buffer 
     * @param[in] font_atlas 
     * @param[in] font_size
     */
    friend void CopyCharacterMapToBuffer(const CharacterMap& character_map, Buffer& buffer, FontAtlas* font_atlas, uint32_t font_size);

  private:
    /// @brief The position of the character map.
    glm::vec2 position_;
    /// @brief The size of the character map, in rows/columns.
    glm::ivec2 size_;
    /// @brief The background colour to use when rendering an empty character.
    glm::vec4 empty_character_bg_colour_;
    /// @brief The character data to be rendered to a game scene.
    CharacterData data_;
  };
}

#endif // ! CHARACTER_MAP_H
