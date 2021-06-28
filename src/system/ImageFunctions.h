/// @author James Holtom

#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <string>

#include "../utility/GLUtils.h"
#include "../utility/SDLUtils.h"

namespace term_engine::system {
  /// Represents an image that has been loaded and bound to a texture in OpenGL.
  struct ImageData {
    /// The OpenGL texture ID for the image.
    GLuint texture_id_;
    /// The path to the image file.
    std::string filename_;
    /// The size of the image.
    glm::ivec2 size_;

    /// Creates an empty set of image data.
    ImageData() : texture_id_(0), filename_(""), size_(glm::ivec2(0)) {}

    /// Creates the image data with the given parameters.
    ImageData(const GLuint& texture_id, const std::string& filename, const glm::ivec2& size) : texture_id_(texture_id), filename_(filename), size_(size) {}
  };

  /// Loads an image to be used as an OpenGL texture.
  /**
   * @param[in] filename The path to the image to load.
   * @returns The loaded image data.
   */
  ImageData CreateImage(const std::string& filename);

  /// Deletes the image texture.
  /**
   * @param[in] image The image data to remove.
   */
  void DeleteImage(const ImageData& image);
}

#endif // ! IMAGE_FUNCTIONS_H
