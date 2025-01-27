#include "Image.h"
#include "../../system/FileFunctions.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/LogUtils.h"

namespace term_engine::usertypes {
  Image::Image(const std::filesystem::path& filepath, rendering::TextureData* data) :
    BaseResource(filepath.string())
  {
    texture_ = rendering::TexturePtr(data);

    utility::LogDebug("Loaded image resource with filepath \"{}\".", name_);
  }

  Image::~Image()
  {
    texture_.reset();

    utility::LogDebug("Destroyed image resource with filepath \"{}\".", name_);
  }

  std::string Image::GetResourceType() const
  {
    return IMAGE_TYPE;
  }

  glm::ivec2 Image::GetTextureSize() const
  {
    return texture_->size_;
  }

  void Image::Use() const
  {
    glActiveTexture(GL_TEXTURE0 + texture_->texture_unit_);
    glBindTexture(GL_TEXTURE_2D, texture_->texture_id_);
  }

  void Image::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Filepath: %s", name_.c_str());
      ImGui::Text("Texture ID: %i", texture_->texture_id_);
      ImGui::Text("Texture Index: %i", texture_->texture_unit_);
      ImGui::Text("Size: %i, %i", texture_->size_.x, texture_->size_.y);
      
      ImGui::TreePop();
    }
  }

  Image* LoadImage(const std::string& filepath)
  {
    std::filesystem::path full_path = system::SearchForResourcePath(filepath);

    if (full_path.empty())
    {
      utility::LogWarn("No image filepath given to load!");

      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(full_path.string());

    if (it != resource_list.end() && it->second->GetResourceType() != std::string(IMAGE_TYPE))
    {
      utility::LogWarn("\"{}\" is the name of a(n) {} resource.", full_path.string(), it->second->GetResourceType());
    }
    else if (it == resource_list.end())
    {
      rendering::TextureData* data = rendering::CreateTextureFromImage(full_path, 1);

      if (data == nullptr)
      {
        utility::LogWarn("Failed to load image at \"{}\"!", full_path.string());
      }

      it = resource_list.emplace(std::make_pair(full_path.string(), std::make_unique<Image>(full_path, data))).first;
    }

    return static_cast<Image*>(it->second.get());
  }
}
