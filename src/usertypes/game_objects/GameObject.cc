#include <sstream>
#include "GameObject.h"
#include "../../events/InputManager.h"
#include "../../events/Listener.h"
#include "../../scripting/ScriptingInterface.h"
#include "../../utility/ConversionUtils.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  GameObject::GameObject(GameScene* game_scene, const glm::ivec2& position, const glm::ivec2& size) :
    BaseObject(),
    layer_(0),
    is_hovering_(false),
    position_(position),
    game_scene_(game_scene)
  {
    data_.SetSize(size);

    utility::logger->debug("Created object with ID {} at {},{} with size {}x{}.", object_id_, position.x, position.y, size.x, size.y);
  }

  GameObject::GameObject(GameScene* game_scene, GameObject* object) :
    BaseObject(object),
    layer_(object->layer_),
    is_hovering_(false),
    position_(object->position_),
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
    if (is_active_ && game_scene_->GetGameWindow() != nullptr)
    {
      GameWindow* game_window = game_scene_->GetGameWindow();
      glm::ivec2 mouse_position = events::GetMousePosition();
      glm::ivec2 mouse_rowcol = utility::GetRowColFromPosition(game_scene_->GetGameWindow(), mouse_position);

      if (game_window->GetWindow()->IsInFocus() && glm::all(glm::greaterThanEqual(mouse_rowcol, position_)) && glm::all(glm::lessThanEqual(mouse_rowcol, position_ + data_.GetSize() - glm::ivec2(1))))
      {
        if (!is_hovering_)
        {
          is_hovering_ = true;
          events::Event ev = events::event_list.emplace_back("object_hover", game_window, scripting::lua_state->create_table_with(
            "game_scene", game_scene_->GetName(),
            "id", object_id_,
            "type", "over"
          ));
        }
      }
      else
      {
        if (is_hovering_)
        {
          is_hovering_ = false;
          
          events::event_list.emplace_back("object_hover", game_window, scripting::lua_state->create_table_with(
            "game_scene", game_scene_->GetName(),
            "id", object_id_,
            "type", "out"
          ));
        }
      }

      if (animation_state_.HasAnimationsQueued())
      {
        animation_state_.Update(timestep);

        AnimationFrame* frame = animation_state_.GetCurrentFrame();
        assert(frame != nullptr);

        game_scene_->GetCharacterMap()->PushCharacters(position_ + frame->GetOffset(), frame->GetCharacterMap());
      }
      else
      {
        game_scene_->GetCharacterMap()->PushCharacters(position_, data_);
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

  GameScene* GameObject::GetGameScene() const
  {
    return game_scene_;
  }

  int32_t GameObject::GetLayer() const
  {
    return layer_;
  }

  bool GameObject::IsHoveringOver() const
  {
    return is_hovering_;
  }

  glm::ivec2& GameObject::GetPosition()
  {
    return position_;
  }

  CharacterMap& GameObject::GetCharacterMap()
  {
    return data_;
  }

  AnimationState& GameObject::GetAnimation()
  {
    return animation_state_;
  }

  void GameObject::SetLayer(int32_t layer)
  {
    layer_ = layer;

    is_object_list_dirty = true;
  }

  void GameObject::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
  }

  void GameObject::SetCharacterMap(const CharacterMap& data)
  {
    data_ = data;
  }

  void GameObject::Set(const sol::function& func)
  {
    data_.SetFunction(func);
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
    is_object_list_dirty = true;

    GameScene* game_scene = GetGameSceneByName(name);

    if (game_scene == nullptr)
    {
      utility::logger->warn("Cannot copy game object to non-existent game scene \"{}\"!", name);

      return nullptr;
    }
    else
    {
      object_list.emplace_front(std::make_shared<GameObject>(game_scene, this));

      return static_cast<GameObject*>(object_list.front().get());
    }
  }

  void GameObject::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s #%li", GetObjectType().c_str(), object_id_))
    {
      ImGui::Text("ID: %lu", object_id_);
      ImGui::Text("Active?: %s", is_active_ ? "Yes" : "No");
      ImGui::Text("Hovering?: %s", is_hovering_ ? "Yes" : "No");
      ImGui::Text("Position: %i, %i", position_.x, position_.y);
      ImGui::Text("Game Scene: %s", game_scene_->GetName().c_str());

      data_.UpdateDebugInfo();
      animation_state_.UpdateDebugInfo();

      ImGui::TreePop();
    }
  }

  GameObject* AddGameObjectToScene(const glm::ivec2& position, const glm::ivec2& size, GameSceneVariant game_scene)
  {
    if (size.x <= 0 || size.y <= 0)
    {
      utility::logger->warn("Cannot create object with a width/height of 0!");

      return nullptr;
    }

    GameScene* find_scene = nullptr;

    try
    {
      if (std::holds_alternative<std::string>(game_scene))
      {
        find_scene = GetGameSceneByName(std::get<std::string>(game_scene));
      }
      else
      {
        find_scene = std::get<GameScene*>(game_scene);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Invalid game scene given to game window!");
    }

    if (find_scene == nullptr)
    {
      utility::logger->warn("Cannot add game object for non-existent game scene \"{}\"!", find_scene->GetName());

      return nullptr;
    }
    else
    {
      is_object_list_dirty = true;

      object_list.emplace_front(std::make_shared<GameObject>(find_scene, position, size));
      
      return static_cast<GameObject*>(object_list.front().get());
    }
  }

  GameObject* AddGameObject(const glm::ivec2& position, const glm::ivec2& size)
  {
    return AddGameObjectToScene(position, size, "default");
  }

  void ClearGameObjectsByGameScene(const std::string& name)
  {
    GameScene* game_scene = GetGameSceneByName(name);

    object_list.remove_if([&game_scene](const ObjectPtr& object)
    {
      return object->GetObjectType() == std::string(GAME_OBJECT_TYPE) && dynamic_cast<GameObject*>(object.get())->GetGameScene() == game_scene;
    });

    utility::logger->debug("Cleared all game objects for game scene \"{}\" from the list.", name);
  }

  GameObject* GetGameObjectById(uint64_t id)
  {
    for (auto& object : object_list)
    {
      if (object->GetObjectId() == id)
      {
        if (object->GetObjectType() == std::string(GAME_OBJECT_TYPE))
        {
          return static_cast<GameObject*>(object.get());
        }
      }
      else
      {
        utility::logger->warn("Object with ID {} is the name of a(n) {}.", id, object->GetObjectType());

        return nullptr;
      }
    }

    return nullptr;
  }
}
