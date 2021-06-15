#include <memory>

#include "ImageFunctions.h"

#include "../logging/Logger.h"

namespace term_engine::system {
  ImageData CreateImage(const std::string& filename)
  {
    SDL::Surface surf_image(SDL_ConvertSurfaceFormat(IMG_Load(filename.c_str()), SDL_PIXELFORMAT_RGBA8888, 0));

    if (surf_image != nullptr) {
      GLuint texture_id = 0;
      glGenTextures(1, &texture_id);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf_image->w, surf_image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf_image->pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);

      logging::logger->info("Loaded image \'{}\'.", filename);

      return ImageData(texture_id, filename, glm::ivec2(surf_image->w, surf_image->h));
    }
    else {
      logging::logger->error("Failed to load image \'{}\'.", filename);

      return ImageData();
    }
  }

  void DeleteImage(const ImageData& image)
  {
    glDeleteTextures(1, &image.texture_id_);

    logging::logger->info("Removed image \'{}\'.", image.filename_);
  }
}
