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

    debug_info_ = utility::AddDebugInfo(GetObjectType() + " " + std::to_string(GetObjectId()));

    utility::logger->debug("Created object with ID {} at {},{} with size {}x{}.", object_id_, position.x, position.y, size.x, size.y);
  }

  GameObject::GameObject(const GameSceneWeakPtr& game_scene, const GameObject* object) :
    BaseObject(),
    position_(object->position_),
    size_(object->size_),
    is_dirty_(object->is_dirty_),
    data_(object->data_),
    game_scene_(game_scene)
  {
    debug_info_ = utility::AddDebugInfo(GetObjectType() + " " + std::to_string(GetObjectId()));

    utility::logger->debug("Copied object with ID {} to ID {}.", object->object_id_, object_id_);
  }

  GameObject::~GameObject()
  {
    utility::RemoveDebugInfo(debug_info_);

    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameObject::Update()
  {
    if (is_active_ && is_dirty_)
    {
      if (!game_scene_.expired())
      {
        GameScenePtr ptr = game_scene_.lock();

        ptr->GetCharacterMap()->PushCharacters(position_, size_, data_);
        ptr->Dirty();

        is_dirty_ = false;
      }
    }
  }

  std::string GameObject::GetObjectType() const
  {
    return std::string(GAME_OBJECT_TYPE);
  }

  ObjectSortPriority GameObject::GetListPriority() const
  {
    return ObjectSortPriority::OBJECT;
  }

  GameSceneWeakPtr GameObject::GetGameScene() const
  {
    return game_scene_;
  }

  glm::ivec2 GameObject::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 GameObject::GetSize() const
  {
    return size_;
  }

  rendering::CharacterData* GameObject::GetData()
  {
    is_dirty_ = true;

    return &data_;
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

  void GameObject::MoveToGameScene(const std::string& name)
  {
    GameScenePtr ptr = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());

    game_scene_ = ptr;
  }

  GameObjectProxy GameObject::CopyToGameScene(const std::string& name)
  {
    is_object_list_dirty_ = true;

    GameScenePtr game_scene = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());

    return GameObjectProxy(object_list_.emplace_front(std::make_shared<GameObject>(game_scene, this)));
  }

  GameObjectProxy::GameObjectProxy(const ObjectPtr& object) :
    BaseProxy(object)
  {}

  GameObjectProxy::~GameObjectProxy()
  {}

  GameObjectProxy AddGameObjectToScene(const glm::ivec2& position, const glm::ivec2& size, const std::string& name)
  {
    is_object_list_dirty_ = true;

    GameScenePtr game_scene = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());
    
    return GameObjectProxy(object_list_.emplace_front(std::make_shared<GameObject>(game_scene, position, size)));
  }

  GameObjectProxy AddGameObject(const glm::ivec2& position, const glm::ivec2& size)
  {
    return AddGameObjectToScene(position, size, "default");
  }

  void ClearAllObjects()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(GAME_OBJECT_TYPE); });

    utility::logger->debug("Cleared all objects from the list.");
  }

  void ClearObjectsByGameScene(const std::string& name)
  {
    GameScenePtr game_scene = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());

    object_list_.remove_if([&game_scene](const ObjectPtr& object) { return object->GetObjectType() == std::string(GAME_OBJECT_TYPE) && std::dynamic_pointer_cast<GameObject>(object)->GetGameScene().lock() == game_scene; });

    utility::logger->debug("Cleared all objects for game scene \"{}\" from the list.", name);
  }
}
