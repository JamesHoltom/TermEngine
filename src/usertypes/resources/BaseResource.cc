#include "BaseResource.h"
#include "Font.h"
#include "ShaderProgram.h"
#include "../../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  BaseResource::BaseResource(const std::string& name) :
    name_(name),
    marked_for_removal_(false) {}

  std::string BaseResource::GetName() const
  {
    return name_;
  }

  bool BaseResource::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void BaseResource::FlagForRemoval()
  {
    if (name_ != std::string(DEFAULT_FONT)
      && name_ != std::string(DEFAULT_TEXT_SHADER)
      && name_ != std::string(DEFAULT_BG_SHADER))
    {
      marked_for_removal_ = true;
    }
  }

  void BaseResource::UnflagForRemoval()
  {
    marked_for_removal_ = false;
  }

  void UpdateResources()
  {
    bool created = false;
    
    if (!utility::test_mode)
    {
      created = ImGui::BeginTabItem("Resources");
    }

    for (auto& [ _, resource ] : resource_list)
    {
      if (created)
      {
        resource->UpdateDebugInfo();
      }
    }

    if (created)
    {
      ImGui::EndTabItem();
    }
  }

  void CleanUpResources()
  {
    resource_list.clear();
  }

  void ClearFlaggedResources()
  {
    for (auto& [ name, resource ] : resource_list)
    {
      if (resource->FlaggedForRemoval())
      {
        resource_list.erase(name);
      }
    }
  }
}