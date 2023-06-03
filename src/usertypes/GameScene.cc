#include "GameScene.h"
#include "game_objects/EventListener.h"
#include "game_objects/GameObject.h"
#include "../system/FileFunctions.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  GameScene::GameScene(const std::string& name) :
    name_(name),
    background_(nullptr),
    font_(nullptr),
    window_(DEFAULT_WINDOW_POSITION, DEFAULT_WINDOW_SIZE, std::string(DEFAULT_WINDOW_TITLE), 0),
    font_size_(DEFAULT_FONT_SIZE),
    character_map_(),
    text_buffer_(),
    marked_for_removal_(false)
  {
    font_ = LoadFont(std::string(DEFAULT_FONT));

    window_.SetSize(font_->GetCharacterSize(font_size_) * character_map_.GetSize());

    background_shader_program_ = GetShader(std::string(DEFAULT_BG_SHADER));
    background_shader_program_->SetUniform("fragment_texture", 1);

    text_shader_program_ = GetShader(std::string(DEFAULT_TEXT_SHADER));
    text_shader_program_->SetUniform("fragment_texture", 0);

    ResetProjection();

    /* TODO: Set debug timing intervals as below:
     *  - Reset Resources
     *  - Window Clear
     *  - Font Update
     *  - BG Update
     *  - BG Draw
     *  - Text Update
     *  - Text Draw
     *  - Window Draw
    */

    utility::logger->debug("Created game scene with name \"{}\".", name_);
  }

  GameScene::~GameScene()
  {
    utility::logger->debug("Destroyed game scene \"{}\".", name_);
  }

  void GameScene::Update()
  {
    // TODO: Figure out how to get execution time here.

    if (font_->FlaggedForRemoval())
    {
      font_ = LoadFont(std::string(DEFAULT_FONT));
    }

    if (text_shader_program_->FlaggedForRemoval())
    {
      text_shader_program_ = GetShader(std::string(DEFAULT_TEXT_SHADER));
    }

    if (background_shader_program_->FlaggedForRemoval())
    {
      background_shader_program_ = GetShader(std::string(DEFAULT_BG_SHADER));
    }

    if (background_ != nullptr && background_->FlaggedForRemoval())
    {
      background_ = nullptr;
    }

    window_.Use();
    window_.Clear();

    glm::ivec2 size = window_.GetSize();
    glViewport(0, 0, size.x, size.y);

    // TODO: See above.

    if (background_ != nullptr)
    {
      background_buffer_.data.clear();
      background_->CopyToBuffer(background_buffer_);

      // TODO: See above.

      background_buffer_.PushToGL();
      background_buffer_.Use();
      background_->Use();
      background_shader_program_->Use();

      glDrawArrays(GL_TRIANGLES, 0, background_buffer_.data.size());

      // TODO: See above.
    }
    else
    {
      // TODO: See above.
    }

    text_buffer_.data.clear();
    CopyCharacterMapToBuffer(character_map_, text_buffer_, font_, font_size_);

    // TODO: See above.

    text_buffer_.PushToGL();
    text_buffer_.Use();
    font_->Use();
    font_->UpdateTexture();
    text_shader_program_->Use();

    glDrawArrays(GL_TRIANGLES, 0, text_buffer_.data.size());

    // TODO: See above.

    window_.Refresh();
    character_map_.Clear();

    // TODO: See above.
  }

  std::string GameScene::GetName() const
  {
    return name_;
  }

  uint32_t GameScene::GetFontSize() const
  {
    return font_size_;
  }

  Background* GameScene::GetBackground()
  {
    return background_;
  }

  CharacterMap* GameScene::GetCharacterMap()
  {
    return &character_map_;
  }

  Font* GameScene::GetFont()
  {
    return font_;
  }

  ShaderProgram* GameScene::GetBackgroundShader()
  {
    return background_shader_program_;
  }

  ShaderProgram* GameScene::GetTextShader()
  {
    return text_shader_program_;
  }

  Window* GameScene::GetWindow()
  {
    return &window_;
  }

  void GameScene::SetBackground(Background* background)
  {
    background_ = background;
  }

  void GameScene::SetFont(Font* font)
  {
    if (font != nullptr)
    {
      font_ = font;
    }

    ResizeToFitCharacterMap();
  }

  void GameScene::SetBackgroundShader(ShaderProgram* shader)
  {
    if (background_shader_program_ != nullptr)
    {
      background_shader_program_ = shader;
    }
  }

  void GameScene::SetTextShader(ShaderProgram* shader)
  {
    if (text_shader_program_)
    {
      text_shader_program_ = shader;
    }
  }

  void GameScene::SetFontSize(uint32_t font_size)
  {
    font_size_ = font_size;

    ResizeToFitCharacterMap();
  }

  void GameScene::ResizeToFitCharacterMap()
  {
    const glm::ivec2 character_size = font_->GetCharacterSize(font_size_);

    window_.SetSize(character_size * character_map_.GetSize());

    ResetProjection();
  }

  void GameScene::ResizeToFitWindow()
  {
    const glm::ivec2 character_size = font_->GetCharacterSize(font_size_);

    ResetProjection();
  }

  bool GameScene::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void GameScene::FlagForRemoval()
  {
    marked_for_removal_ = true;
  }

  void GameScene::UnflagForRemoval()
  {
    marked_for_removal_ = false;
  }

  void GameScene::UpdateDebugInfo()
  {
    if (ImGui::TreeNode(name_.c_str()))
    {
      ImGui::Text("Name: %s", name_.c_str());
      ImGui::Text("Font Size: %i", font_size_);

      window_.UpdateDebugInfo();

      character_map_.UpdateDebugInfo();

      ImGui::SeparatorText("Resources");

      ImGui::Text("Font: %s", font_->GetName().c_str());
      ImGui::Text("Background: %s", background_ != nullptr ? background_->GetName().c_str() : "Not set");

      ImGui::SeparatorText("Shaders");

      ImGui::Text("Text: %s", text_shader_program_->GetName().c_str());
      ImGui::Text("Background: %s", background_shader_program_->GetName().c_str());

      ImGui::TreePop();
    }
  }

  void GameScene::ResetProjection()
  {
    glm::ivec2 window_size = window_.GetSize();
    const glm::mat4 projection = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);
    
    background_shader_program_->SetUniformMatrix("projection", projection, GL_FALSE);
    text_shader_program_->SetUniformMatrix("projection", projection, GL_FALSE);
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
      utility::logger->warn("Game scene \"{}\" already exists!", name);
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

  void UpdateGameScenes()
  {
    bool created = false;
    
    if (!utility::test_mode)
    {
      created = ImGui::BeginTabItem("Game Scenes");
    }

    for (auto& [ name, game_scene ] : game_scene_list)
    {
      game_scene->Update();

      if (created)
      {
        game_scene->UpdateDebugInfo();
      }
    }

    if (created)
    {
      ImGui::EndTabItem();
    }
  }

  void DoGameSceneEvents(const SDL_Event& event)
  {
    for (auto& [ name, game_scene ] : game_scene_list)
    {
      Window* window = game_scene->GetWindow();

      if (event.type == SDL_WINDOWEVENT && event.window.windowID == window->GetId())
      {
        if (event.window.event == SDL_WINDOWEVENT_MOVED)
        {
          window->_SetPosition(glm::ivec2(event.window.data1, event.window.data2));
        }
        else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          window->_SetSize(glm::ivec2(event.window.data1, event.window.data2));
        }
        else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
          if (name == std::string(DEFAULT_GAME_SCENE_NAME))
          {
            quit_flag = true;
          }
          else
          {
            switch (window->GetCloseBehaviour())
            {
              case CloseLogic::HIDE:
                window->Hide();

                utility::logger->debug("Hidden game scene \"{}\".", game_scene->GetName());

                break;
              case CloseLogic::CLOSE:
                game_scene->FlagForRemoval();

                utility::logger->debug("Flagged game scene \"{}\" for removal.", game_scene->GetName());

                break;
              case CloseLogic::QUIT:
                quit_flag = true;

                utility::logger->debug("Call to quit game from game scene \"{}\".", game_scene->GetName());

                break;
            }
          }
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
      else if (object->GetObjectType() == std::string(EVENT_LISTENER_TYPE))
      {
        if (dynamic_cast<EventListener*>(object.get())->GetGameScene()->FlaggedForRemoval())
        {
          object->FlagForRemoval();
        }
      }
    }
  }

  void ClearAllGameScenes()
  {
    game_scene_list.clear();

    utility::logger->debug("Cleared all game scenes from the list.");
  }
}