#include "BaseObject.h"
#include "GameObject.h"
#include "../../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  uint64_t BaseObject::object_next_id_ = 1;

  BaseObject::BaseObject() :
    Flaggable(),
    object_id_(object_next_id_++),
    is_active_(true)
  {}

  BaseObject::BaseObject(BaseObject* object) :
    Flaggable(),
    object_id_(object_next_id_++),
    is_active_(object->is_active_)
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

  uint64_t BaseObject::GetNextId()
  {
    return object_next_id_;
  }

  void SortObjects()
  {
    if (is_object_list_dirty)
    {
      object_list.sort([](const ObjectPtr& lhs, const ObjectPtr& rhs) {
        bool result = lhs->GetListPriority() <= rhs->GetListPriority();

        if (lhs->GetObjectType() == std::string(GAME_OBJECT_TYPE) && rhs->GetObjectType() == std::string(GAME_OBJECT_TYPE))
        {
          GameObject* l_ptr = static_cast<GameObject*>(lhs.get());
          GameObject* r_ptr = static_cast<GameObject*>(rhs.get());

          result = result && l_ptr->GetLayer() <= r_ptr->GetLayer();
        }
        else
        {
          result = result && lhs->GetObjectId() < rhs->GetObjectId();
        }

        return result;
      });

      is_object_list_dirty = false;
    }
  }

  void UpdateObjects(uint64_t timestep) 
  {
    for (ObjectPtr& object : object_list)
    {
      object->Update(timestep);
    }
  }

  void ClearFlaggedObjects()
  {
    object_list.remove_if([](const ObjectPtr& object)
    {
      return object->FlaggedForRemoval();
    });
  }

  void ClearAllObjects()
  {
    object_list.clear();

    utility::logger->debug("Cleared all game objects from the list.");
  }
}