#include <filesystem>
#include "Background.h"
#include "../logging/Logger.h"
#include "../shaders/Shader.h"
#include "../shaders/background.h"
#include "../system/DebugFunctions.h"
#include "../system/FileFunctions.h"
#include "../system/ImageFunctions.h"

namespace term_engine::background {
  GLuint program_id;
  GLuint vao_id;
  GLuint vbo_id;
  system::ImageData current_background;
  BackgroundData data[6];

  void SetBackground(const std::string& file, const glm::vec2& offset, const glm::vec4& colour)
  {
    const std::filesystem::path fullFontPath = system::SearchForResourcePath(file);

    if (!std::filesystem::exists(fullFontPath))
    {
      logging::logger->warn("Attempting to set background to one that doesn't exist: {}", fullFontPath);

      return;
    }

    if (current_background.texture_id_ > 0)
    {
      RemoveBackground();
    }

    current_background = system::CreateImage(fullFontPath);
    if (current_background.texture_id_ > 0)
    {
      data[0] = { offset + glm::vec2(0.0f), glm::vec2(0.0f), colour / 255.0f };
      data[1] = { offset + glm::vec2(current_background.size_), glm::vec2(1.0f), colour / 255.0f };
      data[2] = { offset + glm::vec2(0.0f, current_background.size_.y), glm::vec2(0.0f, 1.0f), colour / 255.0f };
      data[3] = { offset + glm::vec2(0.0f), glm::vec2(0.0f), colour / 255.0f };
      data[4] = { offset + glm::vec2(current_background.size_.x, 0.0f), glm::vec2(1.0f, 0.0f), colour / 255.0f };
      data[5] = { offset + glm::vec2(current_background.size_), glm::vec2(1.0f), colour / 255.0f };

      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
      glBufferData(GL_ARRAY_BUFFER, sizeof(BackgroundData) * 6, &data, GL_DYNAMIC_DRAW);
      
      logging::logger->debug("Loaded background image \'{}\' with size {},{} and ID {}.", current_background.filename_.c_str(), current_background.size_.x, current_background.size_.y, current_background.texture_id_);
    }
    else
    {
      logging::logger->error("Failed to set background image.");
    }
  }

  void RemoveBackground()
  {
    if (current_background.texture_id_ > 0) {
      system::DeleteImage(current_background);

      current_background = system::ImageData();

      logging::logger->debug("Removed background image.");
    }
  }

  void Init()
  {
    CreateBuffers();
    CreateShader();
  }

  void CleanUp()
  {
    RemoveBackground();
    CleanUpBuffers();
    CleanUpShader();
  }

  void CreateBuffers()
  {
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindVertexBuffer(0, vbo_id, 0, sizeof(BackgroundData));

    // Configure the vertex position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, vertex_position_));
    glVertexAttribBinding(0, 0);

    // Configure the texture position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, texture_position_));
    glVertexAttribBinding(1, 0);

    // Configure the colour attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, offsetof(BackgroundData, colour_));
    glVertexAttribBinding(2, 0);

    debug::LogVAOData();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBindVertexArray(0);
  }

  void CreateShader()
  {
    program_id = shaders::CreateProgram();

    shaders::AddShaderString(program_id, GL_VERTEX_SHADER, BACKGROUND_VERT_GLSL);
    shaders::AddShaderString(program_id, GL_FRAGMENT_SHADER, BACKGROUND_FRAG_GLSL);
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

  void Render()
  {
    if (current_background.texture_id_ > 0)
    {
      glUseProgram(program_id);
      glBindVertexArray(vao_id);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, current_background.texture_id_);

      glDrawArrays(GL_TRIANGLES, 0, 6);

      glBindVertexArray(0);
      glUseProgram(0);
    }
  }
}
