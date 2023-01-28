#include "GameObject.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  GameObject::GameObject(const GameSceneWeakPtr& game_scene, const glm::ivec2& position, const glm::ivec2& size) :
    BaseObject(),
    position_(position),
    size_(size),
    game_scene_(game_scene)
  {
    size_t data_size = (size_t)size.x * (size_t)size.y;
    data_.reserve(data_size);
    data_.resize(data_size);
    data_.shrink_to_fit();

    utility::logger->debug("Created object with ID {} at {},{} with size {}x{}.", object_id_, position.x, position.y, size.x, size.y);
  }

  GameObject::~GameObject()
  {
    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameObject::Update()
  {
    if (is_active_ && is_dirty_)
    {
      if (!game_scene_.expired())
      {
        GameScenePtr ptr = game_scene_.lock();

        ptr->GetCharacterMap().PushCharacters(position_, size_, data_);
        ptr->Dirty();

        is_dirty_ = false;
      }
    }
  }

  std::string GameObject::GetObjectType() const
  {
    return "Object";
  }

  glm::ivec2 GameObject::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 GameObject::GetSize() const
  {
    return size_;
  }

  rendering::CharacterData& GameObject::GetData()
  {
    is_dirty_ = true;

    return data_;
  }

  void GameObject::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
    is_dirty_ = true;
  }

  void GameObject::SetSize(const glm::ivec2& size)
  {
    size_t old_data_size = (size_t)size_.x * (size_t)size_.y;
    size_t new_data_size = (size_t)size.x * (size_t)size.y;

    if (old_data_size < new_data_size) {
      data_.reserve(new_data_size);
    }

    data_.resize(new_data_size);
    data_.shrink_to_fit();

    size_ = size;
    is_dirty_ = true;
  }

  void GameObject::Set(const sol::function& func)
  {
    int index = 1;

    for (rendering::CharacterParams& character : data_)
    {
      character = rendering::CharacterParams(func(data_, index++));
    }

    is_dirty_ = true;
  }

  void GameObject::Dirty()
  {
    is_dirty_ = true;
  }

  GameObjectPtr& GameObject::Add(const GameScenePtr& game_scene, const glm::ivec2& position, const glm::ivec2& size)
  {
    is_list_dirty_ = true;

    GameObjectPtr ptr = std::make_shared<GameObject>(game_scene, position, size);

    object_list_.push_front(ptr);
    
    return std::ref(ptr);
  }

  void GameObject::ClearAll()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == "Object"; });

    utility::logger->debug("Cleared all objects from the list.");
  }

  ObjectSortPriority GameObject::GetListPriority() const
  {
    return ObjectSortPriority::OBJECT;
  }
}
