/// @author James Holtom

#ifndef FONT_H
#define FONT_H

#include <filesystem>
#include <map>
#include <string>
#include <variant>
#include <glm/glm.hpp>
#include "BaseResource.h"
#include "../../rendering/Texture.h"
#include "../../rendering/TexturePacker.h"
#include "../../utility/FTUtils.h"

namespace term_engine::usertypes {
  struct CharacterBB;
  class Font;

  /// @brief The type name for Fonts.
  constexpr char FONT_TYPE[] = "Font";
  /// @brief The default font path to use when running the engine.
#if defined(__linux__)
  constexpr char DEFAULT_FONT[] = "truetype/ubuntu/UbuntuMono-R.ttf";
#elif defined(_WIN32) || defined (_WIN64)
  constexpr char DEFAULT_FONT[] = "arial.ttf";
#elif defined(__APPLE__) && defined(__MACH__)
  constexpr char DEFAULT_FONT[] = "Geneva.ttf";
#endif

    /// @brief Used to store a list of loaded font sizes, and their associated character sizes.
  typedef std::map<uint32_t, FT_Size> FontSizeList;
  /// @brief Used to index a character and font size pair.
  typedef std::pair<char16_t, uint32_t> CharacterPair;
  /// @brief Used to store characters and their bounds within the texture.
  typedef std::map<CharacterPair, CharacterBB> CharacterList;
  /// @brief Used to pass either a Font object or it's string index to functions.
  typedef std::variant<Font*, std::string> FontVariant;
  
  /// @brief Defines the bounding position, character/font size and baseline of a character within the font atlas.
  struct CharacterBB {
    /// @brief The position of the character in the font texture.
    glm::ivec2 position_;
    /// @brief The size of the character texture.
    glm::ivec2 character_size_;
    /// @brief The font size.
    uint32_t font_size_;
    /// @brief The baseline for the character, i.e. the vertical distance from the top of the character to where the character sits.
    uint32_t baseline_;

    /**
     * @brief Constructs the character bounding box with the given parameters.
     * 
     * @param[in] position        The position of the bounding box.
     * @param[in] character_size  The size of the character texture.
     * @param[in] font_size       The font size.
     * @param[in] baseline        The baseline for the character.
     */
    CharacterBB(const glm::ivec2& position, const glm::ivec2& character_size, uint32_t font_size, uint32_t baseline) :
      position_(position),
      character_size_(character_size),
      font_size_(font_size),
      baseline_(baseline)
    {}
  };

  /// @brief The default font size to use when running the engine.
  constexpr uint32_t DEFAULT_FONT_SIZE = 32;
  /// @brief The size of the texture to store font characters in.
  constexpr uint32_t TEXTURE_SIZE = 1024;
  /// @brief Defines an empty character that is returned when one fails to load, or a zero-character (i.e. '\0') is loaded.
  const CharacterBB EMPTY_CHARACTER = { glm::ivec2(), glm::ivec2(), 0, 0 };
  /// @brief The bounding box for a blank character, to be used for rendering character backgrounds.
  const CharacterBB whitespace_bbox = { glm::ivec2(0), glm::ivec2(32), 0, 0 };
  
  /// @brief Stores a font resource, used to cache and render characters to a game scene.
  class Font : public BaseResource {
  public:
    /**
     * @brief Constructs the resource with the given filepath.
     * 
     * @param[in] filepath The filepath to the resource.
     * @param[in] face     The font face to load character textures from.
     */
    Font(const std::filesystem::path& filepath, FT_Face face);

    /// @brief Destroys the resource.
    ~Font();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    /**
     * @brief Returns the size of a character texture with the given font size.
     * 
     * @param[in] size The font size.
     * @returns The character texture size.
     */
    glm::ivec2 GetCharacterSize(uint32_t size);

    /**
     * @brief Finds a character in the atlas and returns the texture layer the character is on.
     * @details If no character is found, it is loaded from the font and stored in the atlas.
     * 
     * @param[in] character The character to look up.
     * @param[in] size      The font size of the character, in pixels (px).
     * @returns The bounding box for the character.
     */
    CharacterBB GetCharacter(char16_t character, uint32_t size);

    /// @brief Updates the font texture with newly added characters.
    void UpdateTexture();

    /// @brief Binds the font atlas's texture ID to it's index.
    void Use();

    /// @brief Updates the debugging information for this resource.
    void UpdateDebugInfo() const;

  protected:
    /// @brief A handler for the loaded font face. This also refers to the currently loaded character.
    FT_Face face_;
    /// @brief The list containing all characters loaded from the font.
    CharacterList atlas_;
    /// @brief The texture ID for OpenGL to use when rendering.
    rendering::TexturePtr texture_;
    /// @brief The amount of characters currently stored in the font atlas.
    uint32_t character_count_;
    /// @brief Flag to check if the texture needs refreshing after the atlas has updated.
    bool texture_dirty_;
    /// @brief Stores the bounding boxes for all loaded characters, and packs them into a texture.
    rendering::TexturePacker packer_;
    /// @brief Stores a list of loaded character sizes.
    FontSizeList size_list_;

    /**
     * @brief Creates the texture of a character, and stores it in the atlas texture.
     * 
     * @param[in] character The character to render.
     * @param[in] size      The font size of the character to render, in pixels (px).
     * @returns The bounding box of the loaded character.
     */
    CharacterBB CreateCharTexture(uint64_t character, uint32_t size);

    /**
     * @brief Adds a new set of size metrics to the list of sizes.
     * 
     * @param[in] size The font size to load metrics for.
     * @returns An iterator to the new size, or an invalid iterator if an error occurred.
     */
    FontSizeList::iterator AddSize(uint32_t size);

    /**
     * @brief Sets the size metrics to use when loading characters.
     * 
     * @param[in] size The font size to get metrics for.
     */
    void SetSize(uint32_t size);
  };

  /**
   * @brief Retrives the font resource with the given filepath. If it's not in the list, it will be loaded.
   * 
   * @param[in] filepath The filepath to the font resource.
   * @returns A raw pointer to the resource, or a null pointer if not found.
   */
  Font* LoadFont(const std::string& filepath);
}

#endif // ! FONT_H
