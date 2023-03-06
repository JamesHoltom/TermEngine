/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <filesystem>
#include <memory>
#include <map>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Texture.h"
#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/TexturePacker.h"

namespace term_engine::rendering {
  class FontAtlas;
  struct CharacterBB;

  /// @brief Used to store characters and the texture layers of the character they represent.
  typedef std::unordered_map<FT_UInt32, CharacterBB> CharacterList;

  typedef std::map<FT_UInt, FT_Size> FontSizeList;

  typedef std::shared_ptr<FontAtlas> FontAtlasPtr;

  typedef std::weak_ptr<FontAtlas> FontAtlasWeakPtr;

  typedef std::unordered_map<std::string, FontAtlasPtr> FontAtlasList;

  /// @brief Defines the bounding position, character/font size and baseline of a character within the font atlas.
  struct CharacterBB {

    glm::ivec2 position_;

    glm::ivec2 character_size_;

    FT_UInt font_size_;

    int baseline_;


    CharacterBB(const glm::ivec2& position, const glm::ivec2& character_size, const FT_UInt& font_size, const int& baseline) :
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
  constexpr int DEFAULT_FONT_SIZE = 32;
  /// @brief The size of the texture to store font characters in.
  constexpr GLsizei TEXTURE_SIZE = 1024;
  /// @brief Defines an empty character that is returned when one fails to load, or a zero-character (i.e. '\0') is loaded.
  const CharacterBB EMPTY_CHARACTER = CharacterBB(glm::ivec2(), glm::ivec2(), 0, 0);
  
  /// @brief Represents a cache of font glyphs.
  class FontAtlas {
  public:

    FontAtlas();

    FontAtlas(const std::filesystem::path& filepath);

    ~FontAtlas();

    /**
     * @brief Returns the filepath to the font.
     * 
     * @returns The filepath to the font.
     */
    std::string GetFilePath() const;

    glm::ivec2 GetCharacterSize(const FT_UInt& size);

    /**
     * @brief Finds a character in the atlas and returns the texture layer the character is on.
     * @details If no character is found, it is loaded from the font and stored in the atlas.
     * 
     * @param[in] character The character to look up.
     * @param[in] size      The font size of the character, in pixels (px).
     * @returns The texture layer that holds the character.
     */
    CharacterBB GetCharacter(const FT_ULong& character, const FT_UInt& size);

    CharacterBB GetWhitespace() const;

    bool IsLoaded() const;

    /// @brief Binds the font atlas's texture ID to it's index.
    void Use() const;


    void UpdateTexture();


    static FontAtlasPtr GetFontAtlas(const std::filesystem::path& filepath);


    static void CleanUpFontAtlas();

  private:
    /// @brief The relative filepath of the font being used.
    std::filesystem::path filepath_;
    /// @brief A handler for the loaded font face. This also refers to the currently loaded character.
    FT_Face face_;
    /// @brief The list containing all characters loaded from the font.
    CharacterList atlas_;

    CharacterBB whitespace_bbox_;
    /// @brief The texture ID for OpenGL to use when rendering.
    rendering::TextureData texture_;
    /// @brief The amount of characters currently stored in the font atlas.
    GLuint character_count_;

    bool is_loaded_;

    bool texture_dirty_;

    utility::TexturePacker packer_;

    FontSizeList size_list_;

    static GLuint active_font_;

    static FontAtlasList atlas_cache_;

    /**
     * @brief Creates the texture of a character, and stores it in the atlas texture.
     * 
     * @param[in] character The character to render.
     * @param[in] size      The font size of the character to render, in pixels (px).
     * @returns The bounding box of the loaded character.
     */
    CharacterBB CreateCharTexture(const FT_ULong& character, const FT_UInt& size);


    void SetFontSize(const FT_UInt& size);
  };
}

#endif // ! FONT_ATLAS_H
