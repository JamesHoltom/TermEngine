#include "Background.h"
#include "../../system/FileFunctions.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  Background::Background() :
    filepath_(),
    texture_(),
    position_(glm::ivec2()),
    size_(glm::ivec2()),
    colour_(glm::vec4(255.0f))
  {
    utility::logger->debug("Created background.");
  }

  Background::~Background()
  {
    texture_.reset();

    utility::logger->debug("Destroyed background.");
  }

  glm::ivec2& Background::GetPosition()
  {
    return position_;
  }

  glm::ivec2& Background::GetSize()
  {
    return size_;
  }

  glm::ivec2 Background::GetTextureSize() const
  {
    if (texture_)
    {
      return texture_->size_;
    }
    else
    {
      return glm::ivec2();
    }
  }

  glm::vec4& Background::GetColour()
  {
    return colour_;
  }

  std::string Background::GetSource() const
  {
    return filepath_.string();
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
    if (filepath.empty())
    {
      filepath_ = "";
      texture_.reset();

      utility::logger->debug("Unset background texture.");
      
      return;
    }

    std::filesystem::path fullPath = system::SearchForResourcePath(filepath);

    if (!fullPath.empty())
    {
      rendering::TextureData* ptr = rendering::CreateTextureFromImage(fullPath, 1);

      if (ptr != nullptr)
      {
        filepath_ = fullPath;
        texture_ = rendering::TexturePtr(ptr);
        size_ = texture_->size_;

        utility::logger->debug("Set background texture to \"{}\".", fullPath.string());
      }
      else
      {
        utility::logger->warn("Failed to set background texture to \"{}\"!", fullPath.string());
      }
    }
    else
    {
      utility::logger->warn("Failed to find background texture at \"{}\"!", filepath);
    }
  }

  bool Background::IsLoaded() const
  {
    return (bool)texture_;
  }

  void Background::Reset()
  {
    position_ = glm::ivec2();
    colour_ = glm::vec4(255.0);

    if (texture_)
    {
      size_ = texture_->size_;
    }
    else
    {
      size_ = glm::ivec2();
    }
  }

  void Background::CopyToBuffer(rendering::Buffer& buffer) const
  {
    assert(texture_);

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
    assert(texture_);

    glActiveTexture(GL_TEXTURE0 + texture_->texture_unit_);
    glBindTexture(GL_TEXTURE_2D, texture_->texture_id_);
  }

  void Background::UpdateDebugInfo() const
  {
    ImGui::SeparatorText("Background");

    if (texture_)
    {
      ImGui::Text("Filepath: %s", filepath_.c_str());
      ImGui::Text("Position: %i, %i", position_.x, position_.y);
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Colour: %f, %f, %f, %f", colour_.r, colour_.g, colour_.b, colour_.a);

      ImGui::SeparatorText("Texture");
      ImGui::Text("ID: %i", texture_->texture_id_);
      ImGui::Text("Index: %i", texture_->texture_unit_);
      ImGui::Text("Size: %i, %i", texture_->size_.x, texture_->size_.y);
    }
    else
    {
      ImGui::Text("Not set");
    }
  }
}
