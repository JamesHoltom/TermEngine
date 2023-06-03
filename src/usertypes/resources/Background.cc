#include "Background.h"
#include "../../system/FileFunctions.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  Background::Background(const std::filesystem::path& filepath, const rendering::TextureData& texture) :
    BaseResource(filepath.string()),
    position_(glm::ivec2()),
    colour_(glm::vec4(255.0f)),
    texture_(texture)
  {
    size_ = texture_.size_;

    utility::logger->debug("Created background with {}x{} image \"{}\" at texture ID {}.", size_.x, size_.y, filepath.string(), texture_.texture_id_);
  }

  Background::~Background()
  {
    DeleteTexture(texture_);

    utility::logger->debug("Destroyed background with image \"{}\".", name_);
  }

  std::string Background::GetResourceType() const
  {
    return BACKGROUND_TYPE;
  }

  glm::ivec2& Background::GetPosition()
  {
    return position_;
  }

  glm::ivec2& Background::GetSize()
  {
    return size_;
  }

  glm::ivec2& Background::GetTextureSize()
  {
    return texture_.size_;
  }

  glm::vec4& Background::GetColour()
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
    glActiveTexture(GL_TEXTURE0 + texture_.texture_index_);
    glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);
  }

  void Background::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Filepath: %s", name_.c_str());
      ImGui::Text("Position: %i, %i", position_.x, position_.y);
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Colour: ");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(colour_.r, colour_.g, colour_.b, colour_.a), "%f, %f, %f, %f", colour_.r, colour_.g, colour_.b, colour_.a);

      ImGui::SeparatorText("Texture");
      ImGui::Text("ID: %i", texture_.texture_id_);
      ImGui::Text("Index: %i", texture_.texture_index_);
      ImGui::Text("Size: %i, %i", texture_.size_.x, texture_.size_.y);
      
      ImGui::TreePop();
    }
  }

  Background* LoadBackground(const std::string& filepath)
  {
    std::filesystem::path find_path = system::SearchForResourcePath(filepath);

    if (find_path == "")
    {
      utility::logger->warn("Failed to find background resource at \"{}\".", find_path.string());

      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(find_path.string());

    if (it == resource_list.end())
    {
      rendering::TextureData new_texture = rendering::CreateTextureFromImage(find_path, 1);

      if (new_texture.texture_id_ == 0)
      {
        utility::logger->error("Failed to load background resource at \"{}\".", find_path.string());

        return nullptr;
      }

      it = resource_list.emplace(std::make_pair(find_path.string(), std::make_unique<Background>(find_path.string(), new_texture))).first;
    }

    return static_cast<Background*>(it->second.get());
  }
}
