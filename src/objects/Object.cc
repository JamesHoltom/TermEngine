#include "Object.h"
#include "../logging/Logger.h"

namespace term_engine::objects {
  Object::Object(const std::string& name, const glm::vec2& position, const glm::ivec2& size) :
    name_(name),
    position_(position),
    size_(size)
  {
    size_t data_size = (size_t)size.x * (size_t)size.y;
    data_.reserve(data_size);
    data_.resize(data_size);

    logging::logger->info("Created {}x{} object with {} elements.", size.x, size.y, data_size);
  }

  void Object::Render(glyphs::BufferList& data) const
  {
    for (size_t object_index = 0; object_index < (size_t)size_.x * (size_t)size_.y; ++object_index) {
      glm::ivec2 world_pos = glm::ivec2(position_) + glm::ivec2(object_index % size_.x, object_index / size_.x);
      size_t view_index = ((size_t)glyphs::dimensions.x * (size_t)world_pos.y) + (size_t)world_pos.x;

      // Do not render the glyph if it is obscured from view.
      // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
      if (glm::any(glm::lessThan(world_pos, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(world_pos, glyphs::dimensions))) {
        continue;
      }

      // Do not render the glyph if it is invisible.
      if (data_.at(object_index).character_ == glyphs::NO_CHARACTER) {
        continue;
      }

      data.at(view_index).Set(data_.at(object_index));
    }
  }

  std::string Object::GetName() const
  {
    return name_;
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

  void Object::SetPosition(const glm::vec2& position)
  {
    position_ = position;
    is_dirty = true;
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
    is_dirty = true;
  }

  bool is_dirty = true;
}
