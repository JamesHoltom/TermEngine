/// @author James Holtom

#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <string>
#include "../vendor/stb_image.h"
#include "../utility/GLUtils.h"
#include "../utility/SDLUtils.h"

namespace term_engine::system {
  /// @brief Represents an image that has been loaded and bound to a texture in OpenGL.
  struct ImageData {
    /// @brief The OpenGL texture ID for the image.
    GLuint texture_id_;
    /// @brief The path to the image file.
    std::string filename_;
    /// @brief The size of the image.
    glm::ivec2 size_;

    /// @brief Creates an empty set of image data.
    ImageData() : texture_id_(0), filename_(""), size_(glm::ivec2(0)) {}

    /**
     * @brief Creates the image data with the given parameters.
     * 
     * @param[in] texture_id  The texture ID of the image.
     * @param[in] filename    The filepath to the image.
     * @param[in] size        The size of the image, in pixels (px).
     */
    ImageData(const GLuint& texture_id, const std::string& filename, const glm::ivec2& size) : texture_id_(texture_id), filename_(filename), size_(size) {}
  };

  /**
   * @brief Loads an image to be used as an OpenGL texture.
   * 
   * @param[in] filename The path to the image to load.
   * @returns The loaded image data.
   */
  ImageData CreateImage(const std::string& filename);

  /**
   * @brief Deletes the image texture.
   * 
   * @param[in] image The image data to remove.
   */
  void DeleteImage(const ImageData& image);
}

#endif // ! IMAGE_FUNCTIONS_H
