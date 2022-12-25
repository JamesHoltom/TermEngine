#include "Background.h"
#include "../utility/DebugUtils.h"
#include "../utility/SpdlogUtils.h"

namespace objects {
  Background::Background(const std::filesystem::path& filepath) :
    position_(glm::vec2()),
    colour_(glm::vec4(255.0f)),
    filepath_(filepath)
  {
    texture_ = system::CreateTextureFromImage(filepath);
    size_ = texture_.size_;
  }

  Background::~Background()
  {
    system::DeleteImage(texture_);
  }

  glm::ivec2 Background::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 Background::GetSize() const
  {
    return size_;
  }

  glm::vec4 Background::GetColour() const
  {
    return colour_;
  }

  void Background::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
  }

  void Background::SetSize(const glm::ivec2& size)
  {
    size_ = size;
  }

  void Background::SetColour(const glm::vec4& colour)
  {
    colour_ = colour;
  }

  void Background::Reset()
  {
    position_ = glm::ivec2();
    size_ = texture_.size_;
    colour_ = glm::vec4(255.0);
  }

  void Background::CopyToBuffer(rendering::Buffer& buffer) const
  {
    const glm::vec4 normalised_colour = glm::vec4(colour_) / 255.0f;

    /* Draw order:
     * 1
     * |\
     * | \
     * 3--2
     */
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), texture_.texture_id_, normalised_colour));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), texture_.texture_id_, normalised_colour));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x, position_.y + size_.y), glm::vec2(0.0f, 1.0f), texture_.texture_id_, normalised_colour));

    /* Draw order:
     * 1--2
     *  \ |
     *   \|
     *    3
     */
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), texture_.texture_id_, normalised_colour));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x + size_.x, position_.y), glm::vec2(1.0f, 0.0f), texture_.texture_id_, normalised_colour));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), texture_.texture_id_, normalised_colour));
  }
}
