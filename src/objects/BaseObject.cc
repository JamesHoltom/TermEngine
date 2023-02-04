#include "BaseObject.h"

namespace term_engine::objects {
  bool is_object_list_dirty_ = false;
  ObjectList object_list_;

  std::size_t BaseObject::object_next_id_ = 0;

  BaseObject::BaseObject() :
    object_id_(object_next_id_++),
    is_active_(true)
  {}

  size_t BaseObject::GetObjectId() const
  {
    return object_id_;
  }

  bool BaseObject::IsActive() const
  {
    return is_active_;
  }

  void BaseObject::SetActive(const bool& flag)
  {
    is_active_ = flag;
  }

  BaseProxy::BaseProxy(const ObjectPtr& object) :
    ptr_(object)
  {}

  bool BaseProxy::IsExpired() const
  {
    return ptr_.expired();
  }

  void BaseProxy::Release() const
  {
    if (ptr_.expired())
    {
      utility::logger->warn("Cannot release an already removed object!");

      return;
    }
    
    RemoveObject(ptr_.lock()->GetObjectId());
  }

  void SortObjects()
  {
    if (is_object_list_dirty_)
    {
      object_list_.sort([](const ObjectPtr& lhs, const ObjectPtr& rhs) {
        return lhs->GetListPriority() < rhs->GetListPriority();
      });

      is_object_list_dirty_ = false;
    }
  }

  void RemoveObject(const size_t& id)
  {
    object_list_.remove_if([&id](const ObjectPtr& object) { return object->GetObjectId() == id; });

    utility::logger->info("Removed object with ID {}.", id);
  }

  void CleanUpObjects()
  {
    object_list_.clear();
  }
}