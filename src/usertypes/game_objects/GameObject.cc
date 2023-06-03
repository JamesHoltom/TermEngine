#include <sstream>
#include "GameObject.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  GameObject::GameObject(GameScene* game_scene, const glm::ivec2& position, const glm::ivec2& size) :
    BaseObject(),
    position_(position),
    size_(size),
    game_scene_(game_scene)
  {
    const uint64_t data_size = size.x * size.y;
    data_.reserve(data_size);
    data_.resize(data_size);

    utility::logger->debug("Created object with ID {} at {},{} with size {}x{}.", object_id_, position.x, position.y, size.x, size.y);
  }

  GameObject::GameObject(GameScene* game_scene, GameObject* object) :
    BaseObject(),
    position_(object->position_),
    size_(object->size_),
    data_(object->data_),
    game_scene_(game_scene)
  {
    utility::logger->debug("Copied object with ID {} to ID {}.", object->object_id_, object_id_);
  }

  GameObject::~GameObject()
  {
    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameObject::Update(uint64_t timestep)
  {
    // TODO: Figure out how to get execution time here.

    if (is_active_)
    {
      if (animation_state_.HasAnimationsQueued())
      {
        animation_state_.Update(timestep);

        AnimationFrame* frame = animation_state_.GetCurrentFrame();
        assert(frame != nullptr);

        game_scene_->GetCharacterMap()->PushCharacters(position_ + frame->offset_, frame->size_, frame->data_);
      }
      else
      {
        game_scene_->GetCharacterMap()->PushCharacters(position_, size_, data_);
      }
    }

    // TODO: See above.
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

  glm::ivec2& GameObject::GetPosition()
  {
    return position_;
  }

  glm::ivec2& GameObject::GetSize()
  {
    return size_;
  }

  CharacterData& GameObject::GetData()
  {
    return data_;
  }

  AnimationState& GameObject::GetAnimation()
  {
    return animation_state_;
  }

  void GameObject::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
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
  }

  void GameObject::Set(const sol::function& func)
  {
    int index = 1;

    for (CharacterParams& character : data_)
    {
      character = func.call<CharacterParams>(data_, index++);
    }
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
      object_list_.emplace_front(std::make_shared<GameObject>(game_scene, this));

      return static_cast<GameObject*>(object_list_.front().get());
    }
  }

  void GameObject::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s #%li", GetObjectType().c_str(), object_id_))
    {
      ImGui::Text("ID: %lu", object_id_);
      ImGui::Text("Active?: %s", is_active_ ? "Yes" : "No");
      ImGui::Text("Position: %i, %i", position_.x, position_.y);
      ImGui::Text("Size: %i, %i", size_.x, size_.y);
      ImGui::Text("Game Scene: %s", game_scene_->GetName().c_str());
      
      UpdateCharacterDataDebugInfo(data_);

      animation_state_.UpdateDebugInfo();

      ImGui::TreePop();
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
      object_list_.emplace_front(std::make_shared<GameObject>(game_scene, position, size));
      
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
