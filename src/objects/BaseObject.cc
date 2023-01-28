#include "BaseObject.h"

namespace term_engine::objects {
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

  void BaseObject::Sort()
  {
    if (is_list_dirty_)
    {
      object_list_.sort([](const ObjectPtr& lhs, const ObjectPtr& rhs) {
        return lhs->GetListPriority() < rhs->GetListPriority();
      });

      is_list_dirty_ = false;
    }
  }

  void BaseObject::Remove(ObjectPtr& object)
  {
    object_list_.remove(object);

    object.reset();
  }

  void BaseObject::CleanUp()
  {
    for (ObjectPtr& object : object_list_)
    {
      object.reset();
    }

    object_list_.clear();
  }

  std::size_t BaseObject::object_next_id_ = 0;
  bool BaseObject::is_list_dirty_ = false;
  ObjectList BaseObject::object_list_;
}