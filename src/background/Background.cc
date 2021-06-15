#include "Background.h"

#include "../logging/Logger.h"
#include "../shaders/Shader.h"
#include "../system/ImageFunctions.h"
#include "../utility/DebugFunctions.h"

namespace term_engine::background {
  void SetBackground(const std::string& file)
  {
    if (current_background.texture_id_ > 0) {
      RemoveBackground();
    }

    system::ImageData background = system::CreateImage(file);
    if (background.texture_id_ > 0) {
      current_background = background;

      data.position_ = glm::ivec2(0);
      data.size_ = current_background.size_;
      data.color_ = glm::vec3(255.0f);

      glBindTexture(GL_TEXTURE_2D, current_background.texture_id_);

      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
      glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundData), &data, GL_DYNAMIC_DRAW);
      
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
      logging::logger->error("Failed to set background image.");
    }
  }

  void RemoveBackground()
  {
    if (current_background.texture_id_ > 0) {
      system::DeleteImage(current_background);

      current_background = system::ImageData();
    }
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
    if (current_background.texture_id_ >= 0) {
      glUseProgram(program_id);
      glBindVertexArray(vao_id);
      glBindTexture(GL_TEXTURE_2D, current_background.texture_id_);

      glDrawArrays(GL_POINTS, 0, 1);

      glBindTexture(GL_TEXTURE_2D, 0);
      glBindVertexArray(0);
      glUseProgram(0);
    }
  }

  void CreateBuffers()
  {
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

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

    debug::LogVAOData();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundData), &data, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
  }

  void CreateShader()
  {
    program_id = shaders::CreateProgram();

    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_VERTEX_SHADER, std::string(BG_VERTEX_FILE)));
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_GEOMETRY_SHADER, std::string(BG_GEOMETRY_FILE)));
    shaders::AddShaderFile(program_id, shaders::ShaderInitialisationPair(GL_FRAGMENT_SHADER, std::string(BG_FRAGMENT_FILE)));
    shaders::LinkProgram(program_id);

    glUseProgram(0);
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
  GLuint vao_id;
  GLuint vbo_id;
  system::ImageData current_background;
  BackgroundData data;
}
