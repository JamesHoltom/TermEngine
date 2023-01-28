#include "Buffer.h"
#include "../utility/DebugUtils.h"

namespace term_engine::rendering {
  Buffer::Buffer()
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
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
  }

  Buffer::~Buffer()
  {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
  }

  void Buffer::PushToGL() const
  {
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);
  }

  void Buffer::Use() const
  {
    glBindVertexArray(vao_id_);
  }

  GLuint Buffer::GetVaoId() const
  {
    return vao_id_;
  }

  GLuint Buffer::GetVboId() const
  {
    return vbo_id_;
  }
}