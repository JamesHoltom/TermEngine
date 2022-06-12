#include <memory>
#include "ImageFunctions.h"
#include "../logging/Logger.h"

namespace term_engine::system {
  ImageData CreateImage(const std::string& filename)
  {
    GLuint texture_id = 0;
    int width, height, channels;
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (data != nullptr) {
      GLint mode = GL_RGB;

      if (channels == 4)
      {
        mode = GL_RGBA;
      }

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height);
      glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

      logging::logger->debug("Loaded image \'{}\' with size {},{} and {} channels.", filename, width, height, channels);

      stbi_image_free(data);

      return ImageData(texture_id, filename, glm::ivec2(width, height));
    }
    else {
      logging::logger->error("Failed to load image \'{}\'.\nError: {}", filename, stbi_failure_reason());

      return ImageData();
    }
  }

  void DeleteImage(const ImageData& image)
  {
    glDeleteTextures(1, &image.texture_id_);

    logging::logger->debug("Removed image \'{}\'.", image.filename_);
  }
}
