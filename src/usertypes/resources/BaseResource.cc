#include "BaseResource.h"
#include "Font.h"
#include "ShaderProgram.h"
#include "../../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  BaseResource::BaseResource(const std::string& name) :
    Flaggable(),
    name_(name)
  {}

  std::string BaseResource::GetName() const
  {
    return name_;
  }

  void RemoveResource(BaseResourceVariant resource)
  {
    BaseResource* ptr = nullptr;
    
    try
    {
      if (std::holds_alternative<std::string>(resource))
      {
        ResourceList::iterator it = resource_list.find(std::get<std::string>(resource));

        if (it != resource_list.end())
        {
          ptr = it->second.get();
        }
      }
      else 
      {
        ptr = std::get<BaseResource*>(resource);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Cannot remove invalid resource!");
    }

    if (ptr != nullptr) {
      ptr->FlagForRemoval();
    }
  }

  void CleanUpResources()
  {
    resource_list.clear();

    utility::logger->debug("Cleared all resources from the list.");
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