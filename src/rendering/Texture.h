/// @author James Holtom

#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>
#include "../utility/GLUtils.h"
#include "../vendor/stb_image.h"

namespace term_engine::rendering {
  struct TextureData;
  struct TextureDeleter;

  /// @brief Unique pointer to a texture.
  typedef std::unique_ptr<TextureData, TextureDeleter> TexturePtr;

  /// @brief Represents a texture that has been created in OpenGL.
  struct TextureData {
    /// @brief The OpenGL texture ID for the image.
    uint32_t texture_id_;
    /// @brief The texture unit this texture is set to use.
    uint32_t texture_unit_;
    /// @brief The size of the texture.
    glm::ivec2 size_;

    /// @brief Creates an empty set of texture data.
    TextureData() :
      texture_id_(0),
      texture_unit_(0),
      size_(glm::ivec2(0))
    {}

    /**
     * @brief Creates the texture data with the given parameters.
     * 
     * @param[in] texture_id  The texture ID of the texture.
     * @param[in] size        The size of the texture, in pixels (px).
     * @param[in] unit        The texture unit to bind to.
     */
    TextureData(uint32_t texture_id, const glm::ivec2& size, uint32_t unit) :
      texture_id_(texture_id),
      texture_unit_(unit),
      size_(size)
    {}
  };

  /**
   * @brief Loads an image to be used as an OpenGL texture.
   * 
   * @param[in] filename The path to the image to load.
   * @param[in] unit     The texture unit to bind to.
   * @returns The loaded texture data.
   */
  TextureData* CreateTextureFromImage(const std::filesystem::path& filepath, uint32_t unit);

  /**
   * @brief Allocates an OpenGL texture with a given size.
   * 
   * @param[in] size    The size of the texture to create.
   * @param[in] format  The format of the texture to create.
   * @param[in] unit    The texture unit to bind to.
   * @returns The allocated texture data.
   */
  TextureData* AllocateTexture(const glm::ivec2& size, uint32_t format, uint32_t unit);

  /**
   * @brief Clears the image data for the given texture.
   * 
   * @param[in] texture The texture to clear data from.
   * @param[in] colour  The clear colour to apply to the texture.
   */
  void ClearTexture(TextureData* texture, const glm::vec4& colour);

  /**
   * @brief Deletes the texture.
   * 
   * @param[in] texture The texture data to remove.
   */
  void DeleteTexture(TextureData* texture);

  /**
   * @brief Unbinds the texture at the given texture unit.
   * 
   * @param[in] unit The texture unit to unbind.
   */
  void UnuseTexture(uint32_t unit);

  /// @brief Deleter function for resetting an unique pointer to a Texture.
  struct TextureDeleter {
    /**
     * @brief Allows for deleting a Texture by calling it.
     * 
     * @param[in] ptr Raw pointer to the texture data.
     */
    void operator()(TextureData* ptr) const
    {
      DeleteTexture(ptr);
    }
  };
}

#endif // ! TEXTURE_H
