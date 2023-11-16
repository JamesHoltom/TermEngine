#include "Background.h"
#include "../../system/FileFunctions.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  Background::Background() :
    image_(nullptr),
    position_(glm::ivec2()),
    size_(glm::ivec2()),
    colour_(glm::vec4(255.0f))
  {}

  glm::ivec2& Background::GetPosition()
  {
    return position_;
  }

  glm::ivec2& Background::GetSize()
  {
    return size_;
  }

  glm::vec4& Background::GetColour()
  {
    return colour_;
  }

  std::string Background::GetSource() const
  {
    if (image_ != nullptr)
    {
      return image_->GetName();
    }
    else
    {
      return "";
    }
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
    colour_ = glm::clamp(colour, glm::vec4(0.0f), glm::vec4(255.0f));
  }

  void Background::SetSource(const std::string& filepath)
  {
    Image* img = LoadImage(filepath);

    if (img != nullptr)
    {
      image_ = img;
      size_ = image_->GetTextureSize();
    }
  }

  void Background::RemoveSource()
  {
    image_ = nullptr;
    Reset();
  }

  bool Background::IsLoaded() const
  {
    return image_ != nullptr;
  }

  void Background::Reset()
  {
    position_ = glm::ivec2();
    colour_ = glm::vec4(255.0);

    if (image_)
    {
      size_ = image_->GetTextureSize();
    }
    else
    {
      size_ = glm::ivec2();
    }
  }

  void Background::CopyToBuffer(rendering::Buffer& buffer) const
  {
    assert(image_ != nullptr);

    /* Draw order:
    * 1
    * |\
    * | \
    * 3--2
    */
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), colour_));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), colour_));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x, position_.y + size_.y), glm::vec2(0.0f, 1.0f), colour_));

    /* Draw order:
    * 1--2
    *  \ |
    *   \|
    *    3
    */
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), colour_));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x + size_.x, position_.y), glm::vec2(1.0f, 0.0f), colour_));
    buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), colour_));
  }

  void Background::Use() const
  {
    assert(image_ != nullptr);

    image_->Use();
  }

  void Background::UpdateDebugInfo() const
  {
    ImGui::SeparatorText("Background");

    if (image_ != nullptr)
    {
      ImGui::Text("Image: %s", image_->GetName().c_str());
      ImGui::Text("Position: %i, %i", position_.x, position_.y);
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Colour: %f, %f, %f, %f", colour_.r, colour_.g, colour_.b, colour_.a);
    }
    else
    {
      ImGui::Text("Not set");
    }
  }
}
