#include <memory>
#include "GameScene.h"
#include "GameObject.h"
#include "../shaders/default.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  std::vector<GameSceneNamePair> name_list_;

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
    is_dirty_(true),
    marked_for_removal_(false)
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
      window_.Use();
      window_.Clear();

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

  rendering::GameWindow* GameScene::GetWindow()
  {
    return &window_;
  }

  rendering::FontAtlas* GameScene::GetFontAtlas()
  {
    return &font_atlas_;
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

  void GameScene::Dirty()
  {
    is_dirty_ = true;
  }

  bool GameScene::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void GameScene::FlagRemoval()
  {
    marked_for_removal_ = true;
  }

  void GameScene::UnflagRemoval()
  {
    marked_for_removal_ = false;
  }

  GameSceneProxy::GameSceneProxy(const ObjectPtr& object) :
    BaseProxy(object)
  {}

  GameSceneProxy::~GameSceneProxy()
  {}

  GameSceneProxy AddGameScene(const std::string& name)
  {
    is_object_list_dirty_ = true;

    ObjectPtr ptr = object_list_.emplace_front(std::make_shared<GameScene>(name));

    name_list_.push_back(std::make_pair(name, ptr));
    
    return GameSceneProxy(ptr);
  }

  ObjectWeakPtr GetGameSceneByName(const std::string& name)
  {
    for (const GameSceneNamePair& name_pair : name_list_)
    {
      if (name_pair.first == name)
      {
        return name_pair.second;
      }
    }

    return ObjectWeakPtr();
  }

  void MarkGameSceneForRemoval(const Uint32& window_id)
  {
    for (GameSceneNamePair& game_scene : name_list_)
    {
      ObjectPtr ptr = game_scene.second.lock();

      if (ptr->GetObjectType() == std::string(GAME_SCENE_TYPE))
      {
        GameScenePtr gs_ptr = std::dynamic_pointer_cast<GameScene>(ptr);
        
        if (gs_ptr->GetWindow()->GetId() == window_id)
        {
          gs_ptr->FlagRemoval();

          utility::logger->debug("Flagged game scene \"{}\" for removal.", gs_ptr->GetName());

          break;
        }
      }
    }
  }

  void ClearAllGameScenes()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(GAME_SCENE_TYPE); });

    utility::logger->debug("Cleared all game scenes from the list.");
  }

  void ClearFlaggedGameScenes()
  {
    for (ObjectPtr& object : object_list_)
    {
      GameScenePtr ptr = std::dynamic_pointer_cast<GameScene>(object);

      if (object->GetObjectType() == std::string(GAME_SCENE_TYPE) && ptr->FlaggedForRemoval())
      {
        ClearObjectsByGameScene(ptr->GetName());
      }
    }

    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(GAME_SCENE_TYPE) && std::dynamic_pointer_cast<GameScene>(object)->FlaggedForRemoval(); });
  }
}