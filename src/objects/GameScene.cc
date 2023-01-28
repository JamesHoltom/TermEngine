#include <memory>
#include "GameScene.h"
#include "../shaders/default.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  GameScene::GameScene(const std::string& name) :
    BaseObject(),
    name_(name),
    window_(rendering::DEFAULT_POSITION, rendering::DEFAULT_SIZE, std::string(rendering::DEFAULT_TITLE), 0),
    background_(),
    font_atlas_(rendering::FontAtlas(system::SearchForResourcePath(std::string(rendering::DEFAULT_FONT)), rendering::DEFAULT_FONT_SIZE)),
    character_map_(),
    text_buffer_(),
    background_shader_program_(),
    text_shader_program_(),
    is_dirty_(true)
  {
    window_.SetSize(font_atlas_.GetCharacterSize() * character_map_.GetSize());

    const glm::ivec2 window_size = window_.GetSize();
    const glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_size.x, (GLfloat)window_size.y, 0.0f);

    background_shader_program_.AttachString(shaders::DEFAULT_VERT_GLSL, GL_VERTEX_SHADER);
    background_shader_program_.AttachString(shaders::BACKGROUND_FRAG_GLSL, GL_FRAGMENT_SHADER);
    background_shader_program_.Link();
    background_shader_program_.SetUniform("fragment_texture", 1);
    background_shader_program_.SetUniformMatrix("projection", projection, GL_FALSE);

    text_shader_program_.AttachString(shaders::DEFAULT_VERT_GLSL, GL_VERTEX_SHADER);
    text_shader_program_.AttachString(shaders::TEXT_FRAG_GLSL, GL_FRAGMENT_SHADER);
    text_shader_program_.Link();
    text_shader_program_.SetUniform("fragment_texture", 0);
    text_shader_program_.SetUniformMatrix("projection", projection, GL_FALSE);

    utility::logger->debug("Created game scene with ID {} and name {}.", object_id_, name);
  }

  GameScene::~GameScene()
  {
    utility::logger->debug("Destroyed object with ID {}.", object_id_);
  }

  void GameScene::Update()
  {
    if (is_active_ && is_dirty_)
    {
      background_.CopyToBuffer(background_buffer_);
      character_map_.CopyToBuffer(text_buffer_, font_atlas_);
      text_buffer_.PushToGL();
      background_buffer_.PushToGL();

      if (background_.IsLoaded())
      {
        background_buffer_.Use();
        background_.Use();
        background_shader_program_.Use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
      }

      text_buffer_.Use();
      font_atlas_.Use();
      text_shader_program_.Use();

      // Render the character backgrounds.
      text_shader_program_.SetUniform("is_text", 0);
      glDrawArrays(GL_TRIANGLES, 0, text_buffer_.data.capacity() / 2);

      // Render the character glyphs.
      text_shader_program_.SetUniform("is_text", 1);
      glDrawArrays(GL_TRIANGLES, text_buffer_.data.capacity() / 2, text_buffer_.data.capacity());

      window_.Refresh();

      is_dirty_ = false;
    }
  }

  std::string GameScene::GetObjectType() const
  {
    return "GameScene";
  }

  std::string GameScene::GetName() const
  {
    return name_;
  }

  rendering::Window& GameScene::GetWindow()
  {
    return window_;
  }

  rendering::FontAtlas& GameScene::GetFontAtlas()
  {
    return font_atlas_;
  }

  rendering::Background& GameScene::GetBackground()
  {
    return background_;
  }

  rendering::ShaderProgram& GameScene::GetBackgroundShader()
  {
    return background_shader_program_;
  }

  rendering::ShaderProgram& GameScene::GetTextShader()
  {
    return text_shader_program_;
  }

  rendering::CharacterMap& GameScene::GetCharacterMap()
  {
    return character_map_;
  }

  bool GameScene::IsDirty() const
  {
    return is_dirty_;
  }

  void GameScene::Dirty()
  {
    is_dirty_ = true;
  }

  void GameScene::Clean()
  {
    is_dirty_ = false;
  }

  ObjectPtr& GameScene::Add(const std::string& name)
  {
    is_list_dirty_ = true;
    
    return std::ref(object_list_.emplace_front(std::make_shared<GameScene>(name)));
  }

  void GameScene::ClearAll()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == "GameScene"; });

    utility::logger->debug("Cleared all game scenes from the list.");
  }

  ObjectSortPriority GameScene::GetListPriority() const
  {
    return ObjectSortPriority::GAME_SCENE;
  }

  GameScenePtr default_scene;
}