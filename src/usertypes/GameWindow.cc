#include "GameWindow.h"
#include "../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  GameWindow::GameWindow(bool is_default) :
    Flaggable(),
    game_scene_(nullptr),
    background_(),
    is_default_window_(is_default),
    font_(nullptr),
    font_size_(DEFAULT_FONT_SIZE),
    text_buffer_(),
    background_buffer_(),
    is_closing_(false)
  {
    font_ = LoadFont(std::string(DEFAULT_FONT));

    GameScene* game_scene = nullptr;

    if (is_default_window_)
    {
      window_ = default_window.get();
      window_->SetGameWindow(this);
      game_scene = GetGameSceneByName(DEFAULT_GAME_SCENE_NAME);
      close_logic_ = CloseLogic::QUIT;
    }
    else
    {
      window_ = new Window(this, DEFAULT_WINDOW_POSITION, DEFAULT_WINDOW_SIZE, std::string(DEFAULT_WINDOW_TITLE), 0);
      game_scene = AddGameScene("window_" + std::to_string(window_->GetId()));
      close_logic_ = CloseLogic::CLOSE;
    }

    background_shader_program_ = GetShader(std::string(DEFAULT_BG_SHADER));
    background_shader_program_->SetUniform("fragment_texture", 1);

    text_shader_program_ = GetShader(std::string(DEFAULT_TEXT_SHADER));
    text_shader_program_->SetUniform("fragment_texture", 0);

    SetGameScene(game_scene);
    
    SetProjection(window_->GetSize());

    utility::logger->debug("Created game window.");
  }

  GameWindow::~GameWindow()
  {
    if (game_scene_ != nullptr)
    {
      game_scene_->CallQuit();
      game_scene_->ClearMap();
      game_scene_->UpdateGameWindow(nullptr);
      game_scene_ = nullptr;
    }

    if (!is_default_window_)
    {
      delete window_;
      window_ = nullptr;
    }
    
    utility::logger->debug("Destroyed game window.");
  }

  GameScene* GameWindow::GetGameScene()
  {
    return game_scene_;
  }

  Window* GameWindow::GetWindow()
  {
    return window_;
  }

  Background* GameWindow::GetBackground()
  {
    return &background_;
  }

  Font* GameWindow::GetFont()
  {
    return font_;
  }

  uint32_t GameWindow::GetFontSize() const
  {
    return font_size_;
  }

  ShaderProgram* GameWindow::GetBackgroundShader()
  {
    return background_shader_program_;
  }

  ShaderProgram* GameWindow::GetTextShader()
  {
    return text_shader_program_;
  }

  CloseLogic GameWindow::GetCloseBehaviour() const
  {
    return close_logic_;
  }

  void GameWindow::SetGameScene(GameSceneVariant game_scene)
  {
    GameScene* new_scene = nullptr;

    try
    {
      if (std::holds_alternative<std::string>(game_scene))
      {
        new_scene = GetGameSceneByName(std::get<std::string>(game_scene));
      }
      else
      {
        new_scene = std::get<GameScene*>(game_scene);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Invalid game scene given to game window!");
    }

    if (new_scene != nullptr)
    {
      if (game_scene_ != nullptr)
      {
        game_scene_->CallQuit();
        game_scene_->ClearMap();
        game_scene_->UpdateGameWindow(nullptr);
      }

      if (new_scene->CallInit())
      {
        game_scene_ = new_scene;
        game_scene_->UpdateGameWindow(this);
        
        ResizeToFitCharacterMap();
      }
      else
      {
        game_scene_ = nullptr;
        utility::logger->error("Failed to set up next game scene! Window: {}", window_->GetId());
      }
    }
  }

  void GameWindow::SetFont(FontVariant font)
  {
    Font* new_font = nullptr;

    try
    {
      if (std::holds_alternative<std::string>(font))
      {
        new_font = LoadFont(std::get<std::string>(font));
      }
      else 
      {
        new_font = std::get<Font*>(font);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Invalid font given to game window!");
    }

    if (new_font != nullptr)
    {
      font_ = new_font;

      ResizeToFitCharacterMap();
    }
  }

  void GameWindow::SetFontSize(uint32_t font_size)
  {
    font_size_ = font_size;

    ResizeToFitCharacterMap();
  }

  void GameWindow::SetBackgroundShader(ShaderProgramVariant shader)
  {
    ShaderProgram* new_shader = nullptr;

    try
    {
      if (std::holds_alternative<std::string>(shader))
      {
        new_shader = GetShader(std::get<std::string>(shader));
      }
      else
      {
        new_shader = std::get<ShaderProgram*>(shader);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Invalid background shader given to game window!");
    }

    if (new_shader != nullptr)
    {
      background_shader_program_ = new_shader;
    }
  }

  void GameWindow::SetTextShader(ShaderProgramVariant shader)
  {
    ShaderProgram* new_shader = nullptr;

    try
    {
      if (std::holds_alternative<std::string>(shader))
      {
        new_shader = GetShader(std::get<std::string>(shader));
      }
      else
      {
        new_shader = std::get<ShaderProgram*>(shader);
      }
    }
    catch (const std::bad_variant_access &err)
    {
      utility::logger->error("Invalid text shader given to game window!");
    }

    if (new_shader != nullptr)
    {
      text_shader_program_ = new_shader;
    }
  }

  void GameWindow::SetCloseBehaviour(CloseLogic behaviour)
  {
    if (!is_default_window_) {
      close_logic_ = behaviour;
    }
    else {
      utility::logger->warn("Default window can only be configured to quit!");
    }
  }

  void GameWindow::ReloadGameScene()
  {
    SetGameScene(game_scene_);
  }

  void GameWindow::Update(float timestep)
  {
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

    if (game_scene_ == nullptr)
    {
      return;
    }

    game_scene_->CallLoop(timestep);

    window_->Use();
    window_->Clear();

    glm::ivec2 size = window_->GetSize();
    glViewport(0, 0, size.x, size.y);

    if (background_.IsLoaded())
    {
      background_buffer_.data.clear();
      background_.CopyToBuffer(background_buffer_);
      background_buffer_.PushToGL();
      background_buffer_.Use();
      background_.Use();
      background_shader_program_->Use();

      glDrawArrays(GL_TRIANGLES, 0, background_buffer_.data.size());
    }

    text_buffer_.data.clear();
    CharacterMap::CopyToBuffer(game_scene_->GetCharacterMap(), glm::ivec2(0), text_buffer_, font_, font_size_);

    text_buffer_.PushToGL();
    text_buffer_.Use();
    font_->Use();
    font_->UpdateTexture();
    text_shader_program_->Use();

    glDrawArrays(GL_TRIANGLES, 0, text_buffer_.data.size());

    window_->Refresh();
    game_scene_->ClearMap();
  }

  bool GameWindow::IsClosing() const
  {
    return is_closing_;
  }

  void GameWindow::Close()
  {
    is_closing_ = true;
  }

  void GameWindow::ResizeToFitCharacterMap()
  {
    assert(game_scene_ != nullptr);

    const glm::ivec2 character_size = font_->GetCharacterSize(font_size_);
    const glm::ivec2 map_size = game_scene_->GetCharacterMap()->GetSize();
    const glm::ivec2 window_size(character_size * map_size);

    window_->Resize(window_size);

    SetProjection(window_size);
  }

  void GameWindow::ResizeToFitWindow()
  {
    const glm::ivec2 character_size = font_->GetCharacterSize(font_size_);
  }

  void GameWindow::SetProjection(const glm::ivec2& window_size)
  {
    const glm::mat4 projection = glm::ortho(0.0f, (float)window_size.x, (float)window_size.y, 0.0f);
    
    background_shader_program_->SetUniformMatrix("projection", projection, GL_FALSE);
    text_shader_program_->SetUniformMatrix("projection", projection, GL_FALSE);
  }

  void GameWindow::UpdateDebugInfo()
  {
    std::string name = "window_" + std::to_string(window_->GetId());

    if (ImGui::TreeNode(name.c_str()))
    {
      ImGui::Text("Game Scene: %s", game_scene_ != nullptr ? game_scene_->GetName().c_str() : "Not set");

      ImGui::Text("Font: %s", font_->GetName().c_str());
      ImGui::Text("Font Size: %i", font_size_);

      background_.UpdateDebugInfo();

      window_->UpdateDebugInfo();

      ImGui::SeparatorText("Shaders");

      ImGui::Text("Text: %s", text_shader_program_->GetName().c_str());
      ImGui::Text("Background: %s", background_shader_program_->GetName().c_str());

      ImGui::TreePop();
    }
  }

  bool GameWindow::IsQuitting()
  {
    return quit_flag;
  }

  void GameWindow::Quit()
  {
    quit_flag = true;
  }

  bool GameWindow::quit_flag = false;

  GameWindow* AddGameWindow()
  {
    GameWindowPtr window = game_window_list.emplace_front(std::make_shared<GameWindow>(false));

    return static_cast<GameWindow*>(window.get());
  }

  GameWindow* AddDefaultGameWindow()
  {
    GameWindowPtr window = game_window_list.emplace_front(std::make_shared<GameWindow>(true));

    return static_cast<GameWindow*>(window.get());
  }

  GameWindow* GetGameWindowByWindowId(uint32_t window_id)
  {
    for (GameWindowPtr& game_window : game_window_list)
    {
      if (game_window->GetWindow()->GetId() == window_id)
      {
        return game_window.get();
      }
    }

    utility::logger->warn("Could not find game window with ID {}.", window_id);

    return nullptr;
  }

  void DoGameWindowEvents(const SDL_Event& event)
  {
    for (GameWindowPtr& game_window : game_window_list)
    {
      uint32_t window_id = game_window->GetWindow()->GetId();
      if (event.type == SDL_WINDOWEVENT && event.window.windowID == window_id)
      {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
          switch (game_window->GetCloseBehaviour())
          {
            case CloseLogic::HIDE:
              game_window->GetWindow()->Hide();

              utility::logger->debug("Hidden window \"{}\".", window_id);

              break;
            case CloseLogic::CLOSE:
              game_window->Close();

              utility::logger->debug("Flagged window \"{}\" for removal.", window_id);

              break;
            case CloseLogic::QUIT:
              GameWindow::Quit();

              utility::logger->debug("Call to quit game from window \"{}\".", window_id);

              break;
          }
        }

        game_window->GetWindow()->DoEvents(event);
      }

      if (game_window->IsClosing())
      {
        game_window->FlagForRemoval();
      }
    }
  }

  void UpdateGameWindows(float timestep)
  {
    for (const GameWindowPtr& game_window : game_window_list)
    {
      game_window->Update(timestep);
    }
  }

  void ClearFlaggedGameWindows()
  {
    game_window_list.remove_if([](const GameWindowPtr& game_window)
    {
      return game_window->FlaggedForRemoval();
    });

    if (game_window_list.empty())
    {
      GameWindow::Quit();
    }
  }

  void ClearAllGameWindows()
  {
    game_window_list.clear();

    utility::logger->debug("Cleared all game windows from the list.");
  }
}