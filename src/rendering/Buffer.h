/// @author James Holtom

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include "../utility/GLUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  struct BufferData;
  struct Buffer;

  /// @brief Used to store data in a Buffer.
  typedef std::vector<BufferData> BufferList;

  /// @brief Represents the structure of the buffer used to render characters.
  struct BufferData {
    /**
     * @brief Constructs the buffer data with the given parameters.
     * 
     * @param[in] position          The position of the vertex.
     * @param[in] texture_position  The position of the texture.
     * @param[in] colour            The colour to render the vertex/texture with.
     */
    BufferData(const glm::vec2& position, const glm::vec2& texture_position, const glm::vec4& colour) :
      position_(position),
      texture_position_(texture_position),
      colour_(colour / 255.0f) {}

    /**
     * @brief Allows _std::stringstream_ to correctly parse a _BufferData_ object.
     * 
     * @param[in] os   The stream to output the value to.
     * @param[in] data The data to parse.
     * @returns The updated stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const BufferData& data) {
      return os << std::endl
        << "Vertex position: " << data.position_.x << ", " << data.position_.y << std::endl
        << "Texture position: " << data.texture_position_.x << ", " << data.texture_position_.y << std::endl
        << "Colour: " << data.colour_.r << ", " << data.colour_.g << ", " << data.colour_.b << ", " << data.colour_.a << std::endl;
    }

    /// @brief The position of the vertex.
    glm::vec2 position_;
    /// @brief The position of the texture.
    glm::vec2 texture_position_;
    /// @brief The colour to render the vertex/texture with.
    glm::vec4 colour_;
  };

  class Buffer {
  public:
    /// @brief Constructs the buffer.
    Buffer();

    /// @brief Destroys the buffer.
    ~Buffer();

    /**
     * @brief The buffer of view data to render to the window.
     * 
     * @returns The buffer data.
     */
    BufferList data;

    /// @brief Pushes the buffer data to the VBO.
    void PushToGL() const;

    /// @brief Binds the buffer.
    void Use() const;

    /**
     * @brief Returns the VAO ID for the buffer.
     * 
     * @return The VAO ID.
     */
    GLuint GetVaoId() const;

    /**
     * @brief Returns the VBO ID for the buffer.
     * 
     * @return The VBO ID.
     */
    GLuint GetVboId() const;

  private:
    /// @brief The ID of the VAO used to contain the VBO.
    GLuint vao_id_;
    /// @brief The ID of the VBO used to store the buffer-related data.
    GLuint vbo_id_;
  };
}

#endif // ! BUFFER_H
