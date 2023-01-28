/// @author James Holtom

#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>
#include "../utility/GLUtils.h"
#include "../vendor/stb_image.h"

namespace term_engine::rendering {
  /// @brief Represents a texture that has been created in OpenGL.
  struct TextureData {
    /// @brief The OpenGL texture ID for the image.
    GLuint texture_id_;
    GLuint texture_index_;
    /// @brief The size of the texture.
    glm::ivec2 size_;

    /// @brief Creates an empty set of texture data.
    TextureData() :
      texture_id_(0),
      texture_index_(0),
      size_(glm::ivec2(0))
    {}

    /**
     * @brief Creates the texture data with the given parameters.
     * 
     * @param[in] texture_id  The texture ID of the texture.
     * @param[in] size        The size of the texture, in pixels (px).
     * @param[in] index       The texture index to bind to.
     */
    TextureData(const GLuint& texture_id, const glm::ivec2& size, const GLuint& index) :
      texture_id_(texture_id),
      texture_index_(index),
      size_(size)
    {}
  };

  /**
   * @brief Loads an image to be used as an OpenGL texture.
   * 
   * @param[in] filename The path to the image to load.
   * @param[in] index    The texture index to bind to.
   * @returns The loaded texture data.
   */
  TextureData CreateTextureFromImage(const std::filesystem::path& filepath, const GLuint& index);

  /**
   * @brief Allocates an OpenGL texture with a given size.
   * 
   * @param[in] size    The size of the texture to create.
   * @param[in] format  The format of the texture to create.
   * @param[in] index   The texture index to bind to.
   * @returns The allocated texture data.
   */
  TextureData AllocateTexture(const glm::ivec2& size, const GLenum& format, const GLuint& index);

  /**
   * @brief Clears the image data for the given texture.
   * 
   * @param[in] texture The texture to clear data from.
   * @param[in] colour  The clear colour to apply to the texture.
   */
  void ClearTexture(TextureData& texture, const glm::vec4& colour);

  /**
   * @brief Deletes the texture.
   * 
   * @param[in] texture The texture data to remove.
   */
  void DeleteTexture(const TextureData& texture);

  /**
   * @brief Unbinds the texture at the given index.
   * 
   * @param[in] index The texture index to unbind.
   */
  void UnuseTexture(const GLuint& index);
}

#endif // ! TEXTURE_H
