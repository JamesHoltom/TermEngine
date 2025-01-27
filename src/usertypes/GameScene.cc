#include "EventListener.h"
#include "GameScene.h"
#include "game_objects/GameObject.h"
#include "../system/FileFunctions.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/LogUtils.h"

namespace term_engine::usertypes {
  GameScene::GameScene(const std::string& name) :
    Flaggable(),
    name_(name),
    character_map_(),
    game_window_(nullptr),
    on_init_(sol::nil),
    on_loop_(sol::nil),
    on_quit_(sol::nil)
  {
    utility::LogDebug("Created game scene with name \"{}\".", name_);
  }

  GameScene::~GameScene()
  {
    on_init_ = sol::nil;
    on_loop_ = sol::nil;
    on_quit_ = sol::nil;

    utility::LogDebug("Destroyed game scene \"{}\".", name_);
  }

  std::string GameScene::GetName() const
  {
    return name_;
  }

  CharacterMap* GameScene::GetCharacterMap()
  {
    return &character_map_;
  }

  GameWindow* GameScene::GetGameWindow()
  {
    return game_window_;
  }

  void GameScene::SetGameWindow(GameWindow* game_window)
  {
    if (game_window != nullptr)
    {
      game_window->SetGameScene(this);
    }
  }

  void GameScene::UpdateGameWindow(GameWindow* game_window)
  {
    game_window_ = game_window;
  }

  sol::function* GameScene::GetOnInit()
  {
    return &on_init_;
  }

  sol::function* GameScene::GetOnLoop()
  {
    return &on_loop_;
  }

  sol::function* GameScene::GetOnQuit()
  {
    return &on_quit_;
  }

  void GameScene::SetOnInit(const sol::function& func)
  {
    on_init_ = func;
  }

  void GameScene::SetOnLoop(const sol::function& func)
  {
    on_loop_ = func;
  }

  void GameScene::SetOnQuit(const sol::function& func)
  {
    on_quit_ = func;
  }

  bool GameScene::CallInit()
  {
    utility::LogDebug("Calling init for game scene \"{}\"...", name_);

    if (!on_init_.valid())
    {
      utility::LogDebug("No init found for game scene \"{}\".", name_);

      return true;
    }
    
    return utility::CallWithResult(on_init_);
  }

  void GameScene::CallLoop(float timestep)
  {
    if (on_loop_.valid())
    {
      utility::CallWithTimestep(on_loop_, timestep);
    }
  }

  void GameScene::CallQuit()
  {
    utility::LogDebug("Calling quit for game scene \"{}\"...", name_);

    if (on_quit_.valid())
    {
      utility::Call(on_quit_);
    }
    else
    {
      utility::LogDebug("No quit found for game scene \"{}\".", name_);
    }
  }

  void GameScene::ClearMap()
  {
    character_map_.Clear();
  }

  void GameScene::UpdateDebugInfo()
  {
    if (ImGui::TreeNode(name_.c_str()))
    {
      ImGui::Text("Name: %s", name_.c_str());

      character_map_.UpdateDebugInfo();

      ImGui::TreePop();
    }
  }

  GameScene* AddGameScene(const std::string& name)
  {
    GameSceneList::iterator it = game_scene_list.find(name);

    if (it == game_scene_list.end())
    {
      it = game_scene_list.emplace(std::make_pair(name, std::make_unique<GameScene>(name))).first;
    }
    else
    {
      utility::LogWarn("Game scene \"{}\" already exists!", name);
    }

    return static_cast<GameScene*>(it->second.get());
  }

  GameScene* GetGameSceneByName(const std::string& name)
  {
    for (auto& [ gs_name, game_scene ] : game_scene_list)
    {
      if (gs_name == name)
      {
        return game_scene.get();
      }
    }

    return nullptr;
  }

  void ClearFlaggedGameScenes()
  {
    for (auto it = game_scene_list.cbegin(); it != game_scene_list.cend(); it++)
    {
      if (it->second->FlaggedForRemoval())
      {
        ClearGameObjectsByGameScene(it->first);

        game_scene_list.erase(it);
      }
    }
  }

  void ClearAllGameScenes()
  {
    game_scene_list.clear();

    utility::LogDebug("Cleared all game scenes from the list.");
  }
}