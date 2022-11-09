#include <algorithm>
#include "Object.h"
#include "../logging/Logger.h"

namespace term_engine::objects {
  Object::Object(const glm::ivec2& position, const glm::ivec2& size) :
    object_id_(Object::object_next_id_++),
    position_(position),
    size_(size),
    is_active_(true)
  {
    size_t data_size = (size_t)size.x * (size_t)size.y;
    data_.reserve(data_size);
    data_.resize(data_size);
    data_.shrink_to_fit();

    logging::logger->debug("Created {}x{} object with {} elements.", size.x, size.y, data_size);
  }

  glm::ivec2 Object::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 Object::GetSize() const
  {
    return size_;
  }

  CharacterData& Object::GetData()
  {
    return data_;
  }

  bool Object::IsActive() const
  {
    return is_active_;
  }

  void Object::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
    Object::is_dirty_ = true;
  }

  void Object::SetSize(const glm::ivec2& size)
  {
    size_t old_data_size = (size_t)size_.x * (size_t)size_.y;
    size_t new_data_size = (size_t)size.x * (size_t)size.y;

    if (old_data_size < new_data_size) {
      data_.reserve(new_data_size);
    }

    data_.resize(new_data_size);
    data_.shrink_to_fit();

    size_ = size;
    Object::is_dirty_ = true;
  }

  void Object::SetActive(const bool& flag)
  {
    is_active_ = flag;

    Object::is_dirty_ = true;
  }

  void Object::Set(const sol::function& func)
  {
    for (int index = 0; index < data_.size(); index++)
    {
      func(data_, index + 1, data_.at(index));
    }

    Object::is_dirty_ = true;
  }

  ObjectPtr& Object::Add(const glm::ivec2& position, const glm::ivec2& size)
  {
    logging::logger->debug("Added object with position {},{} and size {},{}", position.x, position.y, size.x, size.y);

    return object_list_.emplace_back(std::make_shared<Object>(position, size));
  }

  ObjectPtr& Object::AddSelf(sol::object self, const glm::ivec2& position, const glm::ivec2& size)
  {
    return Add(position, size);
  }

  void Object::Remove(ObjectPtr& obj)
  {
    ObjectList::iterator it = std::find(object_list_.begin(), object_list_.end(), obj);

    if (it != object_list_.end())
    {
      object_list_.erase(it);

      logging::logger->debug("Removed object. {} refs found.", obj.use_count());
    }
    else
    {
      logging::logger->warn("Couldn't find object in list!");
    }

    obj.reset();
  }

  ObjectList& Object::GetList()
  {
    return object_list_;
  }

  int Object::Count()
  {
    return object_list_.size();
  }

  void Object::CleanUp()
  {
    for (ObjectPtr& obj : object_list_)
    {
      Remove(obj);
    }
  }

  bool Object::IsDirty()
  {
    return Object::is_dirty_;
  }

  void Object::Dirty()
  {
    Object::is_dirty_ = true;
  }

  void Object::Clean()
  {
    Object::is_dirty_ = false;
  }

  int Object::object_next_id_ = 0;
  bool Object::is_dirty_ = true;
  ObjectList Object::object_list_;
}
