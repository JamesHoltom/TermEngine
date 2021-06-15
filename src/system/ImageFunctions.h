/// @author James Holtom

#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <string>

#include "../utility/GLUtils.h"
#include "../utility/SDLUtils.h"

namespace term_engine::system {
  struct ImageData {
    GLuint texture_id_;
    std::string filename_;
    glm::ivec2 size_;

    ImageData() : texture_id_(0), filename_(""), size_(glm::ivec2(0)) {}
    ImageData(const GLuint& texture_id, const std::string& filename, const glm::ivec2& size) : texture_id_(texture_id), filename_(filename), size_(size) {}
  };

  ImageData CreateImage(const std::string& filename);
  void DeleteImage(const ImageData& image);
}

#endif // ! IMAGE_FUNCTIONS_H
