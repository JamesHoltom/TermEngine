#include "Background.h"

#include "../shaders/Shader.h"

namespace term_engine::background {
  void SetBackground(const std::string& file)
  {

  }

  void RemoveBackground()
  {

  }

  int Init()
  {
    CreateBuffers();
    CreateShader();

    return 0;
  }

  void CleanUp()
  {
    CleanUpBuffers();
    CleanUpShader();
  }

  void Render()
  {
    glUseProgram(program_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundData), &data, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, 1);

    glUseProgram(0);
  }

  void CreateBuffers()
  {
    glGenBuffers(1, &vbo_id);
    glBindVertexBuffer(0, vbo_id, 0, sizeof(BackgroundData));

    // Configure the position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, position_));
    glVertexAttribBinding(0, 0);

    // Configure the size attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, size_));
    glVertexAttribBinding(1, 0);

    // Configure the color attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, color_));
    glVertexAttribBinding(2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundData), &data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void CreateShader()
  {
    program_id = shaders::CreateProgram();

    glUseProgram(0);

    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_VERTEX_SHADER, std::string(BG_VERTEX_FILE)));
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_GEOMETRY_SHADER, std::string(BG_GEOMETRY_FILE)));
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_FRAGMENT_SHADER, std::string(BG_FRAGMENT_FILE)));
    shaders::LinkProgram(program_id);
  }

  void CleanUpBuffers()
  {
    glDeleteBuffers(1, &vbo_id);
  }

  void CleanUpShader()
  {
    glDeleteProgram(program_id);
  }

  GLuint program_id;
  GLuint vbo_id;
  std::string filename;
  BackgroundData data;
}
