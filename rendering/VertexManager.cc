#include <spdlog/spdlog.h>

#include "VertexManager.h"

namespace term_engine {
  void CreateVertexData() {
    float vertices[] = {
          0.75f, 0.75f,
          -0.75f, 0.75f,
          0.75f, -0.75f,
          -0.75f, -0.75f
    };

    float elements[] = { 0, 2, 3 };

    glGenVertexArrays(1, &vertex_array_object_id_);
    glBindVertexArray(vertex_array_object_id_);

    glGenBuffers(1, &vertex_buffer_object_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)NULL);

    glGenBuffers(1, &element_buffer_object_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(NULL);

    spdlog::info("Vertex IDs below.\nVAO: {},VBO: {},EBO: {}, Attribute: {}", vertex_array_object_id_, vertex_buffer_object_id_, element_buffer_object_id_, position_attribute_location_);
  }

  GLuint vertex_array_object_id_ = 0;
  GLuint vertex_buffer_object_id_ = 0;
  GLuint element_buffer_object_id_ = 0;
  GLuint position_attribute_location_ = 0;
}