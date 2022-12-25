#include <memory>
#include "ImageFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace system {
  TextureData CreateTextureFromImage(const std::filesystem::path& filepath)
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

    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

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

      logging::logger->debug("Loaded image \'{}\' with size {},{} and {} channels.", filepath.string(), width, height, channels);

      stbi_image_free(data);

      return TextureData(texture_id, glm::ivec2(width, height));
    }
    else {
      logging::logger->error("Failed to load image \'{}\'.\nError: {}", filepath.string(), stbi_failure_reason());

      return TextureData();
    }
  }

  TextureData AllocateTexture(const glm::ivec2& size, const GLenum& format)
  {
    GLuint texture_id = 0;

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, size.x);
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, size.y);

    glTexStorage2D(GL_TEXTURE_2D, 1, format, size.x, size.y);

    logging::logger->debug("Allocated texture with size {},{}.", size.x, size.y);

    return TextureData(texture_id, size);
  }

  void DeleteImage(const TextureData& image)
  {
    logging::logger->debug("Removed texture \'{}\'.", image.texture_id_);

    glDeleteTextures(1, &image.texture_id_);
  }
}
