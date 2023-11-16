/// @author James Holtom

#ifndef CHARACTER_MAP_H
#define CHARACTER_MAP_H

#include "Character.h"
#include "resources/Font.h"
#include "../rendering/Buffer.h"
#include "../utility/GLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::usertypes {
  /// @brief The default number of rows/columns in the view.
  constexpr glm::ivec2 DEFAULT_CHARACTER_MAP_SIZE = glm::ivec2(32, 16);

  /// @brief Defines a map of characters to render to a game scene.
  class CharacterMap {
  public:
    /// @brief Constructs the character map.
    CharacterMap();

    /**
     * @brief Constructs the character map with the given parameters. 
     * 
     * @param[in] size The size of the character map, in rows/columns.
     * @param[in] data The character data to set.
     */
    CharacterMap(const glm::ivec2& size, const sol::table& data);

    /**
     * @brief Gets the size of the character map.
     * 
     * @returns The size of the character map.
     */
    glm::ivec2& GetSize();

    /**
     * @brief Returns the list of characters in the map.
     * 
     * @returns The character data.
     */
    CharacterData& GetData();

    /**
     * @brief Returns if background colour are omitted when rendering empty characters.
     * 
     * @returns If background colours are not drawn for empty characters.
     */
    bool& AreEmptyCharactersHidden();

    /**
     * @brief Sets the size of the character map, in rows/columns.
     * 
     * @param[in] size The new size of the character map.
     */
    void SetSize(const glm::ivec2& size);

    /**
     * @brief Sets the characters in the map with the given data.
     * 
     * @param[in] data The character data to set.
     */
    void SetData(const sol::table& data);

    /**
     * @brief Sets if empty characters should not draw their background colour when rendering.
     * 
     * @param[in] flag If empty characters should not draw their background colour.
     */
    void SetHideEmptyCharacters(bool flag);

    /// @brief Clears the character map.
    void Clear();

    /**
     * @brief Sets the characters in the map, based on the return value from the given Lua function.
     * 
     * @param[in] func The Lua function to generate characters from.
     */
    void SetFunction(const sol::function& func);

    /**
     * @brief Pushes character data to the buffer at the given index.
     * 
     * @param[in] position The position on the map to start pushing characters to, in rows/columns.
     * @param[in] data     The character data to push.
     */
    void PushCharacters(const glm::ivec2& position, const CharacterMap& data);

    /**
     * @brief Copies the data from a character map into an OpenGL buffer.
     * 
     * @param[in] character_map 
     * @param[in] position
     * @param[in,out] buffer 
     * @param[in] font 
     * @param[in] font_size
     */
    static void CopyToBuffer(CharacterMap* character_map, const glm::ivec2& position, rendering::Buffer& buffer, Font* font, uint32_t font_size);

    /// @brief Updates the debugging information for this character map.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The size of the character map, in rows/columns.
    glm::ivec2 size_;
    /// @brief Should the background colour be omitted when rendering an empty character?
    bool hide_empty_characters_;
    /// @brief The character data to be rendered to a game scene.
    CharacterData data_;
  };
}

#endif // ! CHARACTER_MAP_H
