#include "BaseObject.h"
#include "../../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  uint64_t BaseObject::object_next_id_ = 0;

  BaseObject::BaseObject() :
    object_id_(object_next_id_++),
    is_active_(true),
    marked_for_removal_(false)
  {}

  uint64_t BaseObject::GetObjectId() const
  {
    return object_id_;
  }

  bool BaseObject::IsActive() const
  {
    return is_active_;
  }

  void BaseObject::SetActive(bool flag)
  {
    is_active_ = flag;
  }

  bool BaseObject::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void BaseObject::FlagForRemoval()
  {
    marked_for_removal_ = true;
  }

  void BaseObject::UnflagForRemoval()
  {
    marked_for_removal_ = false;
  }

  uint64_t BaseObject::GetNextId()
  {
    return object_next_id_;
  }

  void SortObjects()
  {
    if (is_object_list_dirty_)
    {
      object_list_.sort([](const ObjectPtr& lhs, const ObjectPtr& rhs) {
        return lhs->GetListPriority() < rhs->GetListPriority() && lhs->GetObjectId() < rhs->GetObjectId();
      });

      is_object_list_dirty_ = false;
    }
  }

  void UpdateObjects(uint64_t timestep) 
  {
    bool created = false;
    
    if (!utility::test_mode)
    {
      created = ImGui::BeginTabItem("Game Objects");

      if (created)
      {
        ImGui::Text("Next ID: %lu", BaseObject::GetNextId());
      }
    }

    for (ObjectPtr& object : object_list_)
    {
      object->Update(timestep);
      
      if (created)
      {
        object->UpdateDebugInfo();
      }
    }

    if (created)
    {
      ImGui::EndTabItem();
    }
  }

  void CleanUpObjects()
  {
    object_list_.clear();
  }

  void ClearFlaggedObjects()
  {
    object_list_.remove_if([](const ObjectPtr& object)
    {
      return object->FlaggedForRemoval();
    });
  }
}