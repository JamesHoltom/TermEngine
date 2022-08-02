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

  /// The ID of the background shader program.
  extern GLuint program_id;
  /// The ID of the VAO used to contain the VBO.
  extern GLuint vao_id;
  /// The ID of the VBO used to store the buffer-related data.
  extern GLuint vbo_id;
  /// The pixel data of the background image, if set.
  extern system::ImageData current_background;
  /// The buffer of background data to render to the window.
  extern BackgroundData data[6];

  /// Represents the structure of the buffer used for rendering the background.
  struct BackgroundData {
    /// The position of the vertex, relative to the window.
    glm::vec2 vertex_position_;
    /// The position of the texture, relative to the window.
    glm::vec2 texture_position_;
    /// The colour of the vertex. This will blend with the texture.
    glm::vec3 colour_;
  };

  /// Sets the background image.
  /**
   * @param[in] filename  The path to the image to use.
   * @param[in] offset    Offset to move the background in the window, in pixels (px).
   * @param[in] colour     Colour of the background.
   */
  void SetBackground(const std::string& filename, const glm::vec2& offset, const glm::vec3& colour);

  /// Removes the background image, if set.
  void RemoveBackground();

  /// Prepares the OpenGL buffers and shader ready for use.
  void Init();

  /// Destroys the OpenGL buffers and shader.
  void CleanUp();

  /// Sets up the OpenGL buffers.
  void CreateBuffers();

  /// Sets up the background shader.
  void CreateShader();

  /// Destroys the OpenGL buffers.
  void CleanUpBuffers();

  /// Destroys the background shader.
  void CleanUpShader();

  /// Renders the background to the window.
  void Render();
}

#endif // ! BACKGROUND_H
