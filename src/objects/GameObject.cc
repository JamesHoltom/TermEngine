#include "GameObject.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  GameObject::GameObject(GameScene* game_scene, const glm::ivec2& position, const glm::ivec2& size) :
    BaseObject(),
    position_(position),
    size_(size),
    game_scene_(game_scene)
  {
    const uint64_t data_size = size.x * size.y;
    data_.reserve(data_size);
    data_.resize(data_size);

    debug_info_ = utility::AddDebugInfo(GetObjectType() + " " + std::to_string(GetObjectId()));

    utility::logger->debug("Created object with ID {} at {},{} with size {}x{}.", object_id_, position.x, position.y, size.x, size.y);
  }

  GameObject::GameObject(GameScene* game_scene, GameObject* object) :
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
    debug_info_.reset();

    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameObject::Update()
  {
    debug_info_->Start();

    if (is_active_ && is_dirty_)
    {
      game_scene_->GetCharacterMap()->PushCharacters(position_, size_, data_);

      is_dirty_ = false;
    }

    debug_info_->Interval();
  }

  std::string GameObject::GetObjectType() const
  {
    return std::string(GAME_OBJECT_TYPE);
  }

  ObjectSortPriority GameObject::GetListPriority() const
  {
    return ObjectSortPriority::OBJECT;
  }

  GameScene* GameObject::GetGameScene() const
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
    const uint64_t old_data_size = size_.x * size_.y;
    const uint64_t new_data_size = size.x * size.y;

    if (old_data_size < new_data_size) {
      data_.reserve(new_data_size);
    }

    data_.resize(new_data_size);

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
    GameScene* game_scene = GetGameSceneByName(name);

    if (game_scene == nullptr)
    {
      utility::logger->warn("Cannot move game object to non-existent game scene \"{}\"!", name);
    }
    else
    {
      game_scene_ = game_scene;
    }
  }

  GameObject* GameObject::CopyToGameScene(const std::string& name)
  {
    is_object_list_dirty_ = true;

    GameScene* game_scene = GetGameSceneByName(name);

    if (game_scene == nullptr)
    {
      utility::logger->warn("Cannot copy game object to non-existent game scene \"{}\"!", name);

      return nullptr;
    }
    else
    {
      object_list_.emplace_front(std::make_unique<GameObject>(game_scene, this));

      return static_cast<GameObject*>(object_list_.front().get());
    }
  }

  GameObject* AddGameObjectToScene(const glm::ivec2& position, const glm::ivec2& size, const std::string& name)
  {
    is_object_list_dirty_ = true;

    GameScene* game_scene = GetGameSceneByName(name);

    if (game_scene == nullptr)
    {
      utility::logger->warn("Cannot add game object for non-existent game scene \"{}\"!", name);

      return nullptr;
    }
    else
    {
      object_list_.emplace_front(std::make_unique<GameObject>(game_scene, position, size));
      
      return static_cast<GameObject*>(object_list_.front().get());
    }
  }

  GameObject* AddGameObject(const glm::ivec2& position, const glm::ivec2& size)
  {
    return AddGameObjectToScene(position, size, "default");
  }

  void ClearAllObjects()
  {
    object_list_.remove_if([](const ObjectPtr& object)
    {
      return object->GetObjectType() == std::string(GAME_OBJECT_TYPE);
    });

    utility::logger->debug("Cleared all objects from the list.");
  }
}
