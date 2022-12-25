/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <filesystem>
#include <memory>
#include <vector>
#include "../rendering/Buffer.h"
#include "../system/ImageFunctions.h"
#include "../utility/GLUtils.h"

namespace objects {
  class Background
  {
  public:
    Background(const std::filesystem::path& filepath);

    ~Background();

    glm::ivec2 GetPosition() const;

    glm::ivec2 GetSize() const;

    glm::ivec2 GetTextureSize() const;

    GLuint GetTextureId() const;

    glm::vec4 GetColour() const;

    void SetPosition(const glm::ivec2& position);

    void SetSize(const glm::ivec2& size);

    void SetColour(const glm::vec4& colour);

    void Reset();

    void CopyToBuffer(rendering::Buffer& buffer) const;

  private:
    /// @brief The path to the file containing the texture.
    std::filesystem::path filepath_;
    system::TextureData texture_;
    glm::ivec2 position_;
    glm::ivec2 size_;
    glm::ivec4 colour_;
  };
}

#endif // ! BACKGROUND_H
