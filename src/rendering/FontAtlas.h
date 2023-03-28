/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <filesystem>
#include <memory>
#include <map>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Texture.h"
#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/TexturePacker.h"

namespace term_engine::rendering {
  class FontAtlas;

    /// @brief Used to store a list of loaded font sizes, and their associated character sizes.
  typedef std::map<uint32_t, FT_Size> FontSizeList;
  /// @brief Unique pointer to a font.
  typedef std::unique_ptr<FontAtlas> FontAtlasPtr;
  /// @brief Used to store a list of loaded fonts.
  typedef std::unordered_map<std::string, FontAtlasPtr> FontAtlasList;

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

  /// @brief The default font path to use when running the engine.
#ifdef linux
  constexpr char DEFAULT_FONT[] = "truetype/ubuntu/UbuntuMono-R.ttf";
#elif defined(_WIN32) || defined (WIN32)
  constexpr char DEFAULT_FONT[] = "arial.ttf";
#endif
  /// @brief The default font size to use when running the engine.
  constexpr uint32_t DEFAULT_FONT_SIZE = 32;
  /// @brief The size of the texture to store font characters in.
  constexpr uint32_t TEXTURE_SIZE = 1024;
  /// @brief Defines an empty character that is returned when one fails to load, or a zero-character (i.e. '\0') is loaded.
  const CharacterBB EMPTY_CHARACTER = CharacterBB(glm::ivec2(), glm::ivec2(), 0, 0);
  
  /// @brief Stores a list of loaded fonts.
  extern FontAtlasList atlas_cache;
  
  /// @brief Represents a cache of character textures.
  class FontAtlas {
  public:
    /// @brief Constructs the font atlas with the default font filepath.
    FontAtlas();

    /**
     * @brief Constructs the font atlas with the given font filepath.
     * 
     * @param[in] filepath The filepath to the font.
     */
    FontAtlas(const std::filesystem::path& filepath);

    /// @brief Destroys the font atlas.
    ~FontAtlas();

    /**
     * @brief Returns the filepath to the font.
     * 
     * @returns The filepath to the font.
     */
    std::string GetFilePath() const;

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
    CharacterBB GetCharacter(uint64_t character, uint32_t size);

    /**
     * @brief Returns if the font atlas is fully loaded.
     * 
     * @returns If the font atlas is loaded.
     */
    bool IsLoaded() const;

    /// @brief Binds the font atlas's texture ID to it's index.
    void Use() const;

    /// @brief Updates the font texture with newly added characters.
    void UpdateTexture();

  private:
    /// @brief Used to index a character and font size pair.
    typedef std::pair<uint64_t, uint32_t> CharacterPair;
    /// @brief Used to store characters and their bounds within the texture.
    typedef std::map<CharacterPair, CharacterBB> CharacterList;
    
    /// @brief The relative filepath of the font being used.
    std::filesystem::path filepath_;
    /// @brief A handler for the loaded font face. This also refers to the currently loaded character.
    FT_Face face_;
    /// @brief The list containing all characters loaded from the font.
    CharacterList atlas_;
    /// @brief The texture ID for OpenGL to use when rendering.
    rendering::TextureData texture_;
    /// @brief The amount of characters currently stored in the font atlas.
    uint32_t character_count_;
    /// @brief Internal flag to check if the font atlas has successfully loaded.
    bool is_loaded_;
    /// @brief Flag to check if the texture needs refreshing after the atlas has updated.
    bool texture_dirty_;
    /// @brief Stores the bounding boxes for all loaded characters, and packs them into a texture.
    utility::TexturePacker packer_;
    /// @brief Stores a list of loaded character sizes.
    FontSizeList size_list_;
    /// @brief The ID of the font texture currently in use.
    static uint32_t active_font_id_;

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
   * @brief Returns the font atlas with the given font filepath.
   * @details If a font atlas does not exist, it will be created.
   * 
   * @param[in] filepath The filepath to the font to load.
   * @returns The loaded filepath, or a null pointer if it failed.
   */
  FontAtlas* GetFontAtlas(const std::string& filepath);

  /// @brief Destroys all loaded font atlas.
  void CleanUpFontAtlas();

  /**
   * @brief Returns the default font filepath.
   * 
   * @returns The filepath to the default font.
   */
  std::string GetDefaultFont();

  /**
   * @brief Returns the default font size.
   * 
   * @returns The default font size.
   */
  uint32_t GetDefaultFontSize();

  /// @brief The bounding box for a blank character, to be used for rendering character backgrounds.
  extern CharacterBB whitespace_bbox;
}

#endif // ! FONT_ATLAS_H
