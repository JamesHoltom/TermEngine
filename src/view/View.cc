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
        glyph.Set(default_glyph);
      }

      for (objects::ObjectPtr& object : objects::object_list) {
        if (!object->IsActive()) {
          continue;
        }

        const objects::GlyphData& obj_data = object->GetData();
        const glm::ivec2 obj_size = object->GetSize();

        for (size_t object_index = 0; object_index < (size_t)obj_size.x * (size_t)obj_size.y; ++object_index) {
          glm::ivec2 world_pos = glm::ivec2(object->GetPosition()) + glm::ivec2(object_index % obj_size.x, object_index / obj_size.x);
          size_t view_index = ((size_t)view_size.x * (size_t)world_pos.y) + (size_t)world_pos.x;

          // Do not render the glyph if it is obscured from view.
          // i.e. If the top-left corner of the object is beyond the right/bottom edges of the view, or if the bottom-right corner of the object is beyond the top/left edges of the view.
          if (glm::any(glm::lessThan(world_pos, glm::ivec2(0))) || glm::any(glm::greaterThanEqual(world_pos, view_size))) {
            continue;
          }

          // Do not render the glyph if it is invisible.
          if (obj_data.at(object_index).character_ == NO_CHARACTER) {
            continue;
          }

          data.at(view_index).Set(obj_data.at(object_index));
        }
      }

      objects::Object::Clean();
    }

    glUseProgram(program_id);
    glBindVertexArray(vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(data.capacity()));

    glBindVertexArray(0);
    glUseProgram(0);
  }

  void SetPosition(const glm::vec2& position)
  {
    view_position = position;
    data::SetPosition(position);
  }

  void SetSize(const glm::ivec2& size)
  {
    data.reserve((size_t)size.x * (size_t)size.y);
    data.resize((size_t)size.x * (size_t)size.y);
    data.shrink_to_fit();

    view_size = size;

    size_t count = 0;

    for (BufferData& glyph : data) {
      glyph.position_ = glm::vec2(count % size.x, count / size.x);

      ++count;
    }
  }

  void CreateBuffers()
  {
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindVertexBuffer(0, vbo_id, 0, sizeof(BufferData));

    // Configure the texture layer attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 1, GL_FLOAT, GL_FALSE, offsetof(BufferData, texture_layer_));
    glVertexAttribBinding(0, 0);

    // Configure the glyph position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, position_));
    glVertexAttribBinding(1, 0);

    // Configure the foreground color attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(BufferData, foreground_color_));
    glVertexAttribBinding(2, 0);

    // Configure the background color attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(BufferData, background_color_));
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
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_GEOMETRY_SHADER, std::string(GLYPH_GEOMETRY_FILE)));
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
