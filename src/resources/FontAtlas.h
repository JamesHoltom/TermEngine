/// @author James Holtom

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::fonts {
  struct CharacterBB;

  /// @brief Used to store characters and the texture layers of the character they represent.
  typedef std::unordered_map<FT_UInt32, CharacterBB> CharacterList;

  /// @brief Defines the bounding position, size and baseline of a character within the font atlas.
  struct CharacterBB {
    glm::ivec2 position_;
    glm::ivec2 size_;
    int baseline_;
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
  constexpr CharacterBB EMPTY_CHARACTER = { glm::ivec2(), glm::ivec2() };
  /// @brief The default number of spaces that make up a tab character (i.e. '\\t').
  constexpr unsigned int DEFAULT_TAB_SIZE = 2;
  
  /// @brief The relative filepath of the font being used.
  extern std::string font_path;
  /// @brief The font size, in pixels (px).
  extern int font_size;
  /// @brief The character size, in pixels (px);
  extern glm::ivec2 character_size;
  /// @brief A handler for the loaded font face. This also refers to the currently loaded character.
  extern FT_Face font_face;
  /// @brief The list containing all characters loaded from the font.
  extern CharacterList font_atlas;
  /// @brief The texture ID for OpenGL to use when rendering.
  extern GLuint texture_id;
  /// @brief The amount of characters currently stored in the font atlas.
  extern GLuint character_count;
  /// @brief The position to store the next loaded character at.
  extern glm::uvec2 next_pos;
  /// @brief The tallest height value of a character in the current row. This is used to cleanly move to the next row, after filling the current one.
  extern GLuint max_height;
  /// @brief The amount of spaces to render when rendering a tab character (i.e. '\\t').
  extern unsigned int tab_size;

  /**
   * @brief Initialises the font atlas and prepares it for use.
   * 
   * @returns If the font atlas was successfully set up.
   */
  bool Init();

  /// @brief Destroys the font atlas.
  void CleanUp();

  /**
    * @brief Finds a character in the atlas and returns the texture layer the character is on.
    * @details If no character is found, it is loaded from the font and stored in the atlas.
    * 
    *
    * @param[in] character The character to look up.
    * @returns The texture layer that holds the character.
    */
  CharacterBB GetCharacter(const FT_ULong& character);

  /**
   * @brief Creates the texture of a character, and stores it in the atlas texture.
   * 
   * @param[in] character The character to render.
   * @returns The bounding box of the loaded character.
   */
  std::pair<bool, CharacterBB> _CreateCharTexture(const FT_ULong& character);

  /**
   * @brief Loads a character from the font.
   * 
   * @param[in] character The character to load.
   * @returns The bounding box of the loaded character.
   */
  CharacterBB _LoadChar(const FT_ULong& character);

  /**
   * @brief Sets the font being used to render characters.
   * @details All existing characters in the font atlas are re-rendered when this is called.
   * 
   * @param[in] filename The path to the font file to load.
   * @param[in] size     The size, in pixels (px), to render the characters at.
   * @returns If the font was successfully loaded.
   */
  bool SetFont(const std::string& filename, const FT_Int& size);

  /// @brief Unloads the font currently in use. This is used before loading a new one.
  void _RemoveFont();

  /// @brief Clears the font atlas texture.
  void _ClearCache();

  /**
   * @brief Gets the path to the font currently in use.
   * 
   * @returns The path to the font.
   */
  std::string GetFontPath();

  /**
   * @brief Gets the path to the default font.
   * 
   * @returns The path to the font.
   */
  std::string GetDefaultFontPath();

  /**
   * @brief Gets the font size, in pixels (px).
   * 
   * @returns The font size.
   */
  int GetFontSize();

  /**
   * @brief Gets the default font size, in pixels (px).
   * 
   * @returns The font size.
   */
  int GetDefaultFontSize();

  /**
   * @brief Gets the size of a character, in pixels (px).
   * 
   * @returns The character size.
   */
  glm::ivec2 GetCharacterSize();

  /**
   * @brief Sets the size of a character, in pixels (px).
   * 
   * @param characterSize 
   */
  void SetCharacterSize(const glm::ivec2& characterSize);

  /**
   * @brief Resets the character size, according to the current font.
   */
  void ResetCharacterSize();
}

#endif // ! FONT_ATLAS_H
