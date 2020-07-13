#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

#include "ShaderManager.h"
#include "../gl_includes.h"

namespace term_engine::shaders {
  void InitGlyphShader() {
    ShaderPtr shader = AddShader("glyph");

    shader->BuildShaderFromString(glyph_fragment_shader, GL_FRAGMENT_SHADER);
    shader->BuildShaderFromString(glyph_vertex_shader, GL_VERTEX_SHADER);
    shader->BuildProgram();
  }

  ShaderPtr GetShader(const std::string& name) {
    return shader_list.at(name);
  }

  ShaderPtr AddShader(const std::string& name) {
    auto shader = shader_list.emplace(std::make_pair(name, std::make_shared<Shader>(name)));

    return shader.first->second;
  }

  void RemoveShader(const std::string& name) {
    shader_list.erase(name);
  }

  void CleanUpShaders() {
    shader_list.clear();
  }

  ShaderMap shader_list;

  const GLchar* glyph_fragment_shader = {
    "#version 330 core\n"
    "in vec4 f_color;\n"
    "out vec4 fragment_color;\n"
    "void main() {\n"
    "\tfragment_color = f_color;\n"
    "}\0"
  };

  const GLchar* glyph_vertex_shader = {
    "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "layout (location = 1) in vec2 offset;\n"
    "layout (location = 2) in vec2 scale;\n"
    "layout (location = 3) in vec4 color;\n"
    "out vec4 f_color;\n"
    "void main() {\n"
    "\tf_color = color;\n"
    "\tgl_Position = vec4((position * scale) + offset, 0.0f, 1.0f);\n"
    "}\0"
  };
}
