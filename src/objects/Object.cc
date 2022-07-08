#include "Object.h"
#include "../logging/Logger.h"

namespace term_engine::objects {
  Object::Object(const glm::vec2& position, const glm::ivec2& size) :
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

  glm::vec2 Object::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 Object::GetSize() const
  {
    return size_;
  }

  GlyphData& Object::GetData()
  {
    return data_;
  }

  bool Object::IsActive() const
  {
    return is_active_;
  }

  void Object::SetPosition(const glm::vec2& position)
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
  }

  bool Object::IsDirty()
  {
    return Object::is_dirty_;
  }

  void Object::SetDirty()
  {
    Object::is_dirty_ = true;
  }

  void Object::Clean()
  {
    Object::is_dirty_ = false;
  }

  int Object::object_next_id_ = 0;
  bool Object::is_dirty_ = true;
}
