#include "CharacterMap.h"
#include "../objects/Object.h"
#include "../objects/ShaderProgram.h"
#include "../resources/FontAtlas.h"
#include "../shaders/default.h"
#include "../system/DebugFunctions.h"

namespace term_engine::views {
  glm::vec2 view_position;
  glm::ivec2 view_size;

  void Init()
  {
    CreateBuffers();
    CreateShader();
    SetSize(DEFAULT_DIMENSIONS);
  }

  void CleanUp()
  {
    CleanUpBuffers();
    CleanUpShader();
  }

  void Render()
  {
    if (objects::Object::IsDirty()) {
      for (auto& character : data) {
        character = objects::BufferData();
      }

      for (objects::ObjectPtr& object : objects::Object::GetList()) {
        if (!object->IsActive()) {
          continue;
        }

        const objects::CharacterData& obj_data = object->GetData();
        const glm::ivec2 obj_size = object->GetSize();

        for (size_t object_index = 0; object_index < (size_t)obj_size.x * (size_t)obj_size.y; ++object_index) {
          glm::ivec2 world_pos = object->GetPosition() + glm::ivec2(object_index % obj_size.x, object_index / obj_size.x);
          size_t view_index = (((size_t)view_size.x * (size_t)world_pos.y) + (size_t)world_pos.x) * 12;

          // Do not render the character if it is obscured from view.
          // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
          if (glm::any(glm::lessThan(world_pos, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(world_pos, view_size))) {
            continue;
          }

          // Do not render the character if it is invisible.
          if (obj_data.at(object_index).character_ == objects::NO_CHARACTER) {
            continue;
          }

          PushCharacterToBuffer(view_index, world_pos * fonts::character_size, obj_data.at(object_index));
        }
      }

      objects::Object::Clean();
    }

    glUseProgram(objects::ShaderProgram::GetProgramInUse());
    glBindVertexArray(vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objects::BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fonts::texture_id);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(data.capacity()));

    glBindVertexArray(0);
    glUseProgram(0);
  }

  glm::vec2 GetPosition()
  {
    return view_position;
  }

  void SetPosition(const glm::vec2& position)
  {
    view_position = position;
    data::SetPosition(position);
  }

  glm::ivec2 GetSize()
  {
    return view_size;
  }

  void SetSize(const glm::ivec2& size)
  {
    data.reserve((size_t)size.x * (size_t)size.y * 12);
    data.resize((size_t)size.x * (size_t)size.y * 12);
    data.shrink_to_fit();

    view_size = size;

    objects::Object::Dirty();
  }

  void PushCharacterToBuffer(const size_t& index, const glm::ivec2& position, const objects::CharacterParams& params)
  {
    fonts::CharacterBB bbox = fonts::GetCharacter(params.character_);
    glm::vec2 texPos = glm::vec2(bbox.position_) / (GLfloat)fonts::TEXTURE_SIZE;
    glm::vec2 texSize = glm::vec2(bbox.size_) / (GLfloat)fonts::TEXTURE_SIZE;
    glm::vec2 texAlign = glm::vec2((fonts::character_size.x - bbox.size_.x) / 2, bbox.baseline_);
    bool has_texture = params.character_ != objects::NO_CHARACTER;

    // Draw the background.
    data.at(index) = objects::BufferData(glm::vec2(position), glm::vec2(), false, params.background_colour_);
    data.at(index + 1) = objects::BufferData(glm::vec2(position + fonts::character_size), glm::vec2(), false, params.background_colour_);
    data.at(index + 2) = objects::BufferData(glm::vec2(position.x, position.y + fonts::character_size.y), glm::vec2(), false, params.background_colour_);
    data.at(index + 3) = objects::BufferData(glm::vec2(position), glm::vec2(), false, params.background_colour_);
    data.at(index + 4) = objects::BufferData(glm::vec2(position.x + fonts::character_size.x, position.y), glm::vec2(), false, params.background_colour_);
    data.at(index + 5) = objects::BufferData(glm::vec2(position + fonts::character_size), glm::vec2(), false, params.background_colour_);

    // Draw the foreground.
    data.at(index + 6) = objects::BufferData(glm::vec2(position) + texAlign, texPos, has_texture, params.foreground_colour_);
    data.at(index + 7) = objects::BufferData(glm::vec2(position + bbox.size_) + texAlign, texPos + texSize, has_texture, params.foreground_colour_);
    data.at(index + 8) = objects::BufferData(glm::vec2(position.x, position.y + bbox.size_.y) + texAlign, glm::vec2(texPos.x, texPos.y + texSize.y), has_texture, params.foreground_colour_);
    data.at(index + 9) = objects::BufferData(glm::vec2(position) + texAlign, texPos, has_texture, params.foreground_colour_);
    data.at(index + 10) = objects::BufferData(glm::vec2(position.x + bbox.size_.x, position.y) + texAlign, glm::vec2(texPos.x + texSize.x, texPos.y), has_texture, params.foreground_colour_);
    data.at(index + 11) = objects::BufferData(glm::vec2(position + bbox.size_) + texAlign, texPos + texSize, has_texture, params.foreground_colour_);
  }

  void CreateShader()
  {
    objects::ShaderProgramPtr default_shader = objects::ShaderProgram::Add();
    default_shader->AttachString(DEFAULT_VERT_GLSL, GL_VERTEX_SHADER);
    default_shader->AttachString(DEFAULT_FRAG_GLSL, GL_FRAGMENT_SHADER);
    default_shader->Link();

    glUseProgram(0);
  }
}
