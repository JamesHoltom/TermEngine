/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Texture.h"
#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::rendering {
  struct CharacterBB;

  /// @brief Used to store characters and the texture layers of the character they represent.
  typedef std::unordered_map<FT_UInt32, CharacterBB> CharacterList;

  /// @brief Defines the bounding position, size and baseline of a character within the font atlas.
  struct CharacterBB {
    glm::ivec2 position_;
    glm::ivec2 size_;
    int baseline_;

    CharacterBB(const glm::ivec2& position, const glm::ivec2& size, const int& baseline) :
      position_(position),
      size_(size),
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
  constexpr int DEFAULT_FONT_SIZE = 32;
  /// @brief The size of the texture to store font characters in.
  constexpr GLsizei TEXTURE_SIZE = 1024;
  /// @brief Defines an empty character that is returned when one fails to load, or a zero-character (i.e. '\0') is loaded.
  const CharacterBB EMPTY_CHARACTER = CharacterBB(glm::ivec2(), glm::ivec2(), 0);
  
  class FontAtlas {
  public:
    FontAtlas(const std::filesystem::path& filepath, const FT_Int& size);

    ~FontAtlas();

    /**
     * @brief Returns the filepath to the font.
     * 
     * @returns The filepath to the font.
     */
    std::string GetFilepath() const;

    /**
     * @brief Returns the font size.
     * 
     * @returns The font size.
     */
    FT_Int GetFontSize() const;

    /**
     * @brief Gets the size of a character, in pixels (px).
     * 
     * @returns The character size.
     */
    glm::ivec2 GetCharacterSize() const;

    /**
     * @brief Finds a character in the atlas and returns the texture layer the character is on.
     * @details If no character is found, it is loaded from the font and stored in the atlas.
     * 
     * @param[in] character The character to look up.
     * @returns The texture layer that holds the character.
     */
    CharacterBB GetCharacter(const FT_ULong& character);

    bool IsLoaded() const;

    /**
     * @brief Sets the size of a character, in pixels (px).
     * 
     * @param characterSize 
     */
    void SetCharacterSize(const glm::ivec2& character_size);

    /**
     * @brief Resets the character size, according to the current font.
     */
    void ResetCharacterSize();

    /// @brief Binds the font atlas's texture ID to it's index.
    void Use() const;

  private:
    /// @brief The relative filepath of the font being used.
    std::filesystem::path filepath_;
    /// @brief The font size.
    FT_Int size_;
    /// @brief The character size, in pixels (px);
    glm::ivec2 character_size_;
    /// @brief A handler for the loaded font face. This also refers to the currently loaded character.
    FT_Face face_;
    /// @brief The list containing all characters loaded from the font.
    CharacterList atlas_;
    /// @brief The texture ID for OpenGL to use when rendering.
    TextureData texture_;
    /// @brief The amount of characters currently stored in the font atlas.
    GLuint character_count_;
    /// @brief The position to store the next loaded character at.
    glm::uvec2 next_pos_;
    /// @brief The tallest height value of a character in the current row. This is used to cleanly move to the next row, after filling the current one.
    GLuint max_height_;

    bool is_loaded_;

    /**
     * @brief Creates the texture of a character, and stores it in the atlas texture.
     * 
     * @param[in] character The character to render.
     * @returns The bounding box of the loaded character.
     */
    CharacterBB CreateCharTexture(const FT_ULong& character);

    /// @brief Clears the font atlas texture.
    void ClearCache();
  };
}

#endif // ! FONT_ATLAS_H
