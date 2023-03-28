#include <memory>
#include "GameScene.h"
#include "GameObject.h"
#include "../shaders/default.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  std::vector<GameScene*> GameScene::scene_list_;

  bool GameScene::quit_flag_ = false;

  GameScene::GameScene(const std::string& name) :
    BaseObject(),
    name_(name),
    window_(rendering::DEFAULT_POSITION, rendering::DEFAULT_SIZE, std::string(rendering::DEFAULT_TITLE), 0),
    background_(),
    font_atlas_(),
    font_size_(rendering::DEFAULT_FONT_SIZE),
    character_map_(),
    text_buffer_(),
    background_shader_program_(),
    text_shader_program_()
  {
    font_atlas_ = rendering::GetFontAtlas(std::string(rendering::DEFAULT_FONT));
    window_.SetSize(font_atlas_->GetCharacterSize(font_size_) * character_map_.GetSize());

    background_shader_program_.AttachString(shaders::DEFAULT_VERT_GLSL, GL_VERTEX_SHADER);
    background_shader_program_.AttachString(shaders::BACKGROUND_FRAG_GLSL, GL_FRAGMENT_SHADER);
    background_shader_program_.Link();
    background_shader_program_.SetUniform("fragment_texture", 1);

    text_shader_program_.AttachString(shaders::DEFAULT_VERT_GLSL, GL_VERTEX_SHADER);
    text_shader_program_.AttachString(shaders::TEXT_FRAG_GLSL, GL_FRAGMENT_SHADER);
    text_shader_program_.Link();
    text_shader_program_.SetUniform("fragment_texture", 0);

    ResetProjection();

    debug_info_ = utility::AddDebugInfo(GetObjectType() + " " + std::to_string(GetObjectId()));
    debug_info_->AddSubItem("Window", 1);
    debug_info_->AddSubItem("Font Update", 1);
    debug_info_->AddSubItem("BG Update", 1);
    debug_info_->AddSubItem("BG Draw", 1);
    debug_info_->AddSubItem("Text Update", 1);
    debug_info_->AddSubItem("Text Draw", 1);
    debug_info_->AddSubItem("Refresh", 1);

    utility::logger->debug("Created game scene with ID {} and name {}.", object_id_, name);
  }

  GameScene::~GameScene()
  {
    debug_info_.reset();

    scene_list_.erase(std::remove(scene_list_.begin(), scene_list_.end(), this), scene_list_.end());

    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameScene::Update()
  {
    if (is_active_)
    {
      debug_info_->Start();

      window_.Use();
      window_.Clear();

      debug_info_->Interval(0);

      font_atlas_->UpdateTexture();

      debug_info_->Interval(1);

      if (background_.IsLoaded())
      {
        background_buffer_.data.clear();
        background_.CopyToBuffer(background_buffer_);

        debug_info_->Interval(2);

        background_buffer_.PushToGL();
        
        background_buffer_.Use();
        background_.Use();
        background_shader_program_.Use();

        glDrawArrays(GL_TRIANGLES, 0, background_buffer_.data.size());

        debug_info_->Interval(3);
      }
      else
      {
        debug_info_->Reset(2);
        debug_info_->Reset(3);
      }

      text_buffer_.data.clear();
      CopyCharacterMapToBuffer(character_map_, text_buffer_, font_atlas_, font_size_);

      debug_info_->Interval(4);

      text_buffer_.PushToGL();

      text_buffer_.Use();
      font_atlas_->Use();
      text_shader_program_.Use();

      glDrawArrays(GL_TRIANGLES, 0, text_buffer_.data.size());

      debug_info_->Interval(5);

      window_.Refresh();

      debug_info_->Interval(6);
    }
  }

  std::string GameScene::GetObjectType() const
  {
    return std::string(GAME_SCENE_TYPE);
  }

  ObjectSortPriority GameScene::GetListPriority() const
  {
    return ObjectSortPriority::GAME_SCENE;
  }

  std::string GameScene::GetName() const
  {
    return name_;
  }

  rendering::FontAtlas* GameScene::GetFontAtlas()
  {
    return font_atlas_;
  }

  uint32_t GameScene::GetFontSize() const
  {
    return font_size_;
  }

  rendering::Background* GameScene::GetBackground()
  {
    return &background_;
  }

  rendering::ShaderProgram* GameScene::GetBackgroundShader()
  {
    return &background_shader_program_;
  }

  rendering::ShaderProgram* GameScene::GetTextShader()
  {
    return &text_shader_program_;
  }

  rendering::CharacterMap* GameScene::GetCharacterMap()
  {
    return &character_map_;
  }

  rendering::GameWindow* GameScene::GetWindow()
  {
    return &window_;
  }

  void GameScene::SetFontAtlas(rendering::FontAtlas* font_atlas)
  {
    font_atlas_ = font_atlas;
  }

  void GameScene::SetFontSize(uint32_t font_size)
  {
    font_size_ = font_size;

    ResizeToFitCharacterMap();
  }

  void GameScene::ResizeToFitCharacterMap() const
  {
    const glm::ivec2 character_size = font_atlas_->GetCharacterSize(font_size_);

    window_.SetSize(character_size * character_map_.GetSize());

    ResetProjection();
  }

  void GameScene::ResetProjection() const
  {
    const glm::ivec2 window_size = window_.GetSize();
    const glm::mat4 projection = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);
    
    background_shader_program_.SetUniformMatrix("projection", projection, GL_FALSE);
    text_shader_program_.SetUniformMatrix("projection", projection, GL_FALSE);
  }

  GameScene* AddGameScene(const std::string& name)
  {
    is_object_list_dirty_ = true;

    object_list_.emplace_front(std::make_unique<GameScene>(name));

    return GameScene::scene_list_.emplace_back(static_cast<GameScene*>(object_list_.front().get()));
  }

  GameScene* GetGameSceneByName(const std::string& name)
  {
    for (GameScene* game_scene : GameScene::scene_list_)
    {
      if (game_scene->GetName() == name)
      {
        return game_scene;
      }
    }

    return nullptr;
  }

  void FlagGameSceneForRemoval(uint32_t window_id)
  {
    for (GameScene*& game_scene : GameScene::scene_list_)
    {
      rendering::GameWindow* window = game_scene->GetWindow();

      if (window->GetId() == window_id)
      {
        if (game_scene->GetName() == std::string(DEFAULT_GAME_SCENE_NAME))
        {
          GameScene::quit_flag_ = true;
        }

        switch (window->GetCloseBehaviour())
        {
          case rendering::CloseLogic::HIDE:
            window->Hide();

            utility::logger->debug("Hid game scene \"{}\".", game_scene->GetName());

            break;
          case rendering::CloseLogic::CLOSE:
            game_scene->FlagForRemoval();

            utility::logger->debug("Flagged game scene \"{}\" for removal.", game_scene->GetName());

            break;
          case rendering::CloseLogic::QUIT:
            GameScene::quit_flag_ = true;

            utility::logger->debug("Call to quit game from game scene \"{}\".", game_scene->GetName());

            break;
        }

        break;
      }
    }
  }

  void FlagGameObjectsWithFlaggedGameScenes()
  {
    for (ObjectPtr& object : object_list_)
    {
      if (object->GetObjectType() == std::string(GAME_OBJECT_TYPE))
      {
        if (dynamic_cast<GameObject*>(object.get())->GetGameScene()->FlaggedForRemoval())
        {
          object->FlagForRemoval();
        }
      }
    }
  }

  void ClearAllGameScenes()
  {
    object_list_.remove_if([](const ObjectPtr& object)
    {
      return object->GetObjectType() == std::string(GAME_SCENE_TYPE);
    });

    utility::logger->debug("Cleared all game scenes from the list.");
  }
}