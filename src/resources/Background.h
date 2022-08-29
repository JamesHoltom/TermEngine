/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <string>
#include "../system/ImageFunctions.h"
#include "../utility/GLUtils.h"

namespace term_engine::background {
  struct BackgroundData;

  /// @brief The location of the vertex GLSL file for the background shader.
  constexpr char BG_VERTEX_FILE[] = "resources/shaders/core/background.vert";
  /// @brief The location of the geometry GLSL file for the background shader.
  constexpr char BG_GEOMETRY_FILE[] = "resources/shaders/core/background.geom";
  /// @brief The location of the fragment GLSL file for the background shader.
  constexpr char BG_FRAGMENT_FILE[] = "resources/shaders/core/background.frag";

  /// @brief The ID of the background shader program.
  extern GLuint program_id;
  /// @brief The ID of the VAO used to contain the VBO.
  extern GLuint vao_id;
  /// @brief The ID of the VBO used to store the buffer-related data.
  extern GLuint vbo_id;
  /// @brief The pixel data of the background image, if set.
  extern system::ImageData current_background;
  /// @brief The buffer of background data to render to the window.
  extern BackgroundData data[6];

  /// @brief Represents the structure of the buffer used for rendering the background.
  struct BackgroundData {
    /// @brief The position of the vertex, relative to the window.
    glm::vec2 vertex_position_;
    /// @brief The position of the texture, relative to the window.
    glm::vec2 texture_position_;
    /// @brief The colour of the vertex. This will blend with the texture.
    glm::vec3 colour_;
  };

  /**
   * @brief Sets the background image.
   * 
   * @param[in] filename  The path to the image to use.
   * @param[in] offset    Offset to move the background in the window, in pixels (px).
   * @param[in] colour    Colour of the background.
   */
  void SetBackground(const std::string& filename, const glm::vec2& offset, const glm::vec3& colour);

  /// @brief Removes the background image, if set.
  void RemoveBackground();

  /// @brief Prepares the OpenGL buffers and shader ready for use.
  void Init();

  /// @brief Destroys the OpenGL buffers and shader.
  void CleanUp();

  /// @brief Sets up the OpenGL buffers.
  void CreateBuffers();

  /// @brief Sets up the background shader.
  void CreateShader();

  /// @brief Destroys the OpenGL buffers.
  void CleanUpBuffers();

  /// @brief Destroys the background shader.
  void CleanUpShader();

  /// @brief Renders the background to the window.
  void Render();
}

#endif // ! BACKGROUND_H
