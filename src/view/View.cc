#include "View.h"
#include "../data/Uniform.h"
#include "../objects/Object.h"
#include "../objects/ObjectManager.h"
#include "../shaders/Shader.h"
#include "../utility/DebugFunctions.h"
#include "../utility/Glyph.h"

namespace term_engine::views {
  GLuint program_id;
  GLuint vao_id;
  GLuint vbo_id;
  glm::vec2 view_position;
  glm::ivec2 view_size;
  BufferList data;

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
      for (auto& glyph : data) {
        glyph = BufferData();
      }

      for (objects::ObjectPtr& object : objects::object_list) {
        if (!object->IsActive()) {
          continue;
        }

        const objects::GlyphData& obj_data = object->GetData();
        const glm::ivec2 obj_size = object->GetSize();

        for (size_t object_index = 0; object_index < (size_t)obj_size.x * (size_t)obj_size.y; ++object_index) {
          glm::ivec2 world_pos = glm::ivec2(object->GetPosition()) + glm::ivec2(object_index % obj_size.x, object_index / obj_size.x);
          size_t view_index = (((size_t)view_size.x * (size_t)world_pos.y) + (size_t)world_pos.x) * 12;

          // Do not render the glyph if it is obscured from view.
          // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
          if (glm::any(glm::lessThan(world_pos, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(world_pos, view_size))) {
            continue;
          }

          // Do not render the glyph if it is invisible.
          if (obj_data.at(object_index).character_ == NO_CHARACTER) {
            continue;
          }

          PushGlyphToBuffer(view_index, world_pos * fonts::glyph_size, obj_data.at(object_index));
        }
      }

      objects::Object::Clean();
    }

    glUseProgram(program_id);
    glBindVertexArray(vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

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

    objects::Object::SetDirty();
  }

  void PushGlyphToBuffer(const size_t& index, const glm::ivec2& position, const GlyphParams& params)
  {
    fonts::GlyphBB bbox = fonts::GetCharacter(params.character_);
    glm::vec2 texPos = glm::vec2(bbox.position_) / (GLfloat)fonts::TEXTURE_SIZE;
    glm::vec2 texSize = glm::vec2(bbox.size_) / (GLfloat)fonts::TEXTURE_SIZE;
    glm::vec2 texAlign = glm::vec2((fonts::glyph_size.x - bbox.size_.x) / 2, bbox.baseline_);
    bool has_texture = params.character_ != NO_CHARACTER;

    // Draw the background.

    data.at(index) = BufferData(glm::vec2(position), glm::vec2(), false, params.background_color_);
    data.at(index + 1) = BufferData(glm::vec2(position + fonts::glyph_size), glm::vec2(), false, params.background_color_);
    data.at(index + 2) = BufferData(glm::vec2(position.x, position.y + fonts::glyph_size.y), glm::vec2(), false, params.background_color_);
    data.at(index + 3) = BufferData(glm::vec2(position), glm::vec2(), false, params.background_color_);
    data.at(index + 4) = BufferData(glm::vec2(position.x + fonts::glyph_size.x, position.y), glm::vec2(), false, params.background_color_);
    data.at(index + 5) = BufferData(glm::vec2(position + fonts::glyph_size), glm::vec2(), false, params.background_color_);

    // Draw the foreground.

    data.at(index + 6) = BufferData(glm::vec2(position) + texAlign, texPos, has_texture, params.foreground_color_);
    data.at(index + 7) = BufferData(glm::vec2(position + bbox.size_) + texAlign, texPos + texSize, has_texture, params.foreground_color_);
    data.at(index + 8) = BufferData(glm::vec2(position.x, position.y + bbox.size_.y) + texAlign, glm::vec2(texPos.x, texPos.y + texSize.y), has_texture, params.foreground_color_);
    data.at(index + 9) = BufferData(glm::vec2(position) + texAlign, texPos, has_texture, params.foreground_color_);
    data.at(index + 10) = BufferData(glm::vec2(position.x + bbox.size_.x, position.y) + texAlign, glm::vec2(texPos.x + texSize.x, texPos.y), has_texture, params.foreground_color_);
    data.at(index + 11) = BufferData(glm::vec2(position + bbox.size_) + texAlign, texPos + texSize, has_texture, params.foreground_color_);
  }

  void CreateBuffers()
  {
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindVertexBuffer(0, vbo_id, 0, sizeof(BufferData));

    // Configure the vertex position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(BufferData, position_));
    glVertexAttribBinding(0, 0);

    // Configure the "Has texture?" attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 1, GL_FLOAT, GL_FALSE, offsetof(BufferData, has_texture_));
    glVertexAttribBinding(1, 0);

    // Configure the texture position attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, texture_position_));
    glVertexAttribBinding(2, 0);

    // Configure the color attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(BufferData, color_));
    glVertexAttribBinding(3, 0);

    debug::LogVAOData();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
  }

  void CreateShader()
  {
    program_id = shaders::CreateProgram();

    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_VERTEX_SHADER, std::string(GLYPH_VERTEX_FILE)));
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_FRAGMENT_SHADER, std::string(GLYPH_FRAGMENT_FILE)));
    shaders::LinkProgram(program_id);

    glUseProgram(0);
  }

  void CleanUpBuffers()
  {
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
  }

  void CleanUpShader()
  {
    glDeleteProgram(program_id);
  }
}
