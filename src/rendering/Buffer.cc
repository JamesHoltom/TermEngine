#include "Buffer.h"
#include "../utility/DebugUtils.h"

namespace term_engine::rendering {
  Buffer::Buffer() :
    current_data_size_(0)
  {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    glGenBuffers(1, &vbo_id_);
    glBindVertexBuffer(0, vbo_id_, 0, sizeof(BufferData));

    // Configure the vertex position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(BufferData, position_));
    glVertexAttribBinding(0, 0);

    // Configure the texture position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, texture_position_));
    glVertexAttribBinding(1, 0);

    // Configure the colour attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, offsetof(BufferData, colour_));
    glVertexAttribBinding(2, 0);

    utility::LogVAOData();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

    glBindVertexArray(0);
  }

  Buffer::~Buffer()
  {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
  }

  void Buffer::PushToGL()
  {
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);

    // If the size of the buffer has changed, recreate the OpenGL buffer with the new size.
    if (current_data_size_ != data.size())
    {
      current_data_size_ = data.size();
      glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * current_data_size_, data.data(), GL_STREAM_DRAW);

      utility::logger->debug("Resized buffer at VAO {} to {} items...", vao_id_, current_data_size_);
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BufferData) * current_data_size_, data.data());
  }

  void Buffer::Use() const
  {
    glBindVertexArray(vao_id_);
  }

  uint32_t Buffer::GetVaoId() const
  {
    return vao_id_;
  }

  uint32_t Buffer::GetVboId() const
  {
    return vbo_id_;
  }
}