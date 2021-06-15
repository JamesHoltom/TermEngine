/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>

#include "../system/ImageFunctions.h"
#include "../utility/GLUtils.h"

namespace term_engine::background {
  struct BackgroundData;

  /// The location of the vertex GLSL file for the background shader.
  constexpr char BG_VERTEX_FILE[] = "resources/shaders/core/background.vert";
  /// The location of the geometry GLSL file for the background shader.
  constexpr char BG_GEOMETRY_FILE[] = "resources/shaders/core/background.geom";
  /// The location of the fragment GLSL file for the background shader.
  constexpr char BG_FRAGMENT_FILE[] = "resources/shaders/core/background.frag";

  struct BackgroundData {
    glm::vec2 position_;
    glm::vec2 size_;
    glm::vec3 color_;
  };

  void SetBackground(const std::string& filename);
  void RemoveBackground();

  int Init();

  void CleanUp();

  void Render();

  void CreateBuffers();

  void CreateShader();

  void CleanUpBuffers();

  void CleanUpShader();

  /// The ID of the background shader program.
  extern GLuint program_id;
  /// The ID of the VAO used to contain the VBO.
  extern GLuint vao_id;
  /// The ID of the VBO used to store the buffer-related data.
  extern GLuint vbo_id;

  extern system::ImageData current_background;

  extern BackgroundData data;
}

#endif // ! BACKGROUND_H
