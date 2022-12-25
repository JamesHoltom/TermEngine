/// @author James Holtom

#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <filesystem>
#include "../vendor/stb_image.h"
#include "../utility/GLUtils.h"

namespace system {
  /// @brief Represents a texture that has been created in OpenGL.
  struct TextureData {
    /// @brief The OpenGL texture ID for the image.
    GLuint texture_id_;
    /// @brief The size of the texture.
    glm::ivec2 size_;

    /// @brief Creates an empty set of texture data.
    TextureData() : texture_id_(0), size_(glm::ivec2(0)) {}

    /**
     * @brief Creates the texture data with the given parameters.
     * 
     * @param[in] texture_id  The texture ID of the texture.
     * @param[in] size        The size of the texture, in pixels (px).
     */
    TextureData(const GLuint& texture_id, const glm::ivec2& size) : texture_id_(texture_id), size_(size) {}
  };

  /**
   * @brief Loads an image to be used as an OpenGL texture.
   * 
   * @param[in] filename The path to the image to load.
   * @returns The loaded texture data.
   */
  TextureData CreateTextureFromImage(const std::filesystem::path& filepath);

  /**
   * @brief Allocates an OpenGL texture with a given size.
   * 
   * @param[in] size    The size of the texture to create.
   * @param[in] format  The format of the texture to create.
   * @returns The allocated texture data.
   */
  TextureData AllocateTexture(const glm::ivec2& size, const GLenum& format);

  /**
   * @brief Deletes the texture.
   * 
   * @param[in] texture The texture data to remove.
   */
  void DeleteImage(const TextureData& texture);
}

#endif // ! IMAGE_FUNCTIONS_H
