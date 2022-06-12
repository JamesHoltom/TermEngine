#include <memory>

#include "ImageFunctions.h"

#include "../logging/Logger.h"

namespace term_engine::system {
  ImageData CreateImage(const std::string& filename)
  {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

    if (data != nullptr) {
      GLuint texture_id = 0;

      glGenTextures(1, &texture_id);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glBindTexture(GL_TEXTURE_2D, 0);

      logging::logger->debug("Loaded image \'{}\'.", filename);

      stbi_image_free(data);

      return ImageData(texture_id, filename, glm::ivec2(width, height));
    }
    else {
      logging::logger->error("Failed to load image \'{}\'.", filename);

      return ImageData();
    }
  }

  void DeleteImage(const ImageData& image)
  {
    glDeleteTextures(1, &image.texture_id_);

    logging::logger->debug("Removed image \'{}\'.", image.filename_);
  }
}
