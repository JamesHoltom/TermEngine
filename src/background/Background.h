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
  extern BackgroundData data;

  /// Represents the structure of the buffer used for rendering the background.
  struct BackgroundData {
    /// The position of the background, relative to the window.
    glm::vec2 position_;
    /// The size of the background, in pixels.
    glm::vec2 size_;
    /// The color of the background. This will blend with the texture.
    glm::vec3 color_;
  };

  /// Sets the background image.
  /**
   * @param[in] filename The path to the image to use.
   */
  void SetBackground(const std::string& filename);

  /// Removes the background image, if set.
  void RemoveBackground();

  /// Prepares the OpenGL buffers and shader ready for use.
  /**
   * @returns If the OpenGL buffers and shader were successfully set up.
   */
  int Init();

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
