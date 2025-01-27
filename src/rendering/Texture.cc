#include <memory>
#include "Texture.h"
#include "../utility/LogUtils.h"

namespace term_engine::rendering {
  TextureData* CreateTextureFromImage(const std::filesystem::path& filepath, uint32_t unit)
  {
    uint32_t texture_id = 0;
    int width, height, channels;
    uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if (data != nullptr) {
      int mode = GL_RGB;

      if (channels == 4)
      {
        mode = GL_RGBA;
      }

      glGenTextures(1, &texture_id);
      glActiveTexture(GL_TEXTURE0 + unit);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, height);
      glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

      utility::LogDebug("Loaded image \'{}\' with size {},{} and {} channels.", filepath.string(), width, height, channels);

      stbi_image_free(data);

      return new TextureData(texture_id, glm::ivec2(width, height), unit);
    }
    else {
      utility::LogError("Failed to load image \'{}\'.\nError: {}", filepath.string(), stbi_failure_reason());

      return nullptr;
    }
  }

  TextureData* AllocateTexture(const glm::ivec2& size, uint32_t format, uint32_t unit)
  {
    assert(size.x > 0 && size.y > 0);
    
    uint32_t texture_id = 0;

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, size.x);
    glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, size.y);

    glTexStorage2D(GL_TEXTURE_2D, 1, format, size.x, size.y);

    utility::LogDebug("Allocated texture with size {},{}.", size.x, size.y);

    return new TextureData(texture_id, size, unit);
  }

  void ClearTexture(TextureData* texture, const glm::vec4& colour)
  {
    if (texture != nullptr)
    {
      glm::vec4 clamped_colour = glm::clamp(colour, glm::vec4(0.0f), glm::vec4(255.0f));
      uint8_t clear_colour[4] = { static_cast<uint8_t>(clamped_colour.r), static_cast<uint8_t>(clamped_colour.g), static_cast<uint8_t>(clamped_colour.b), static_cast<uint8_t>(clamped_colour.a) };

      glActiveTexture(GL_TEXTURE0 + texture->texture_unit_);
      glBindTexture(GL_TEXTURE_2D, texture->texture_id_);
      glClearTexImage(texture->texture_id_, 0, GL_RED, GL_UNSIGNED_BYTE, &clear_colour);
    }
  }

  void DeleteTexture(TextureData* texture)
  {
    if (texture != nullptr && texture->texture_id_ > -1)
    {
      utility::LogDebug("Removed texture \'{}\'.", texture->texture_id_);

      glDeleteTextures(1, &texture->texture_id_);
    }
  }

  void UnuseTexture(uint32_t unit)
  {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
