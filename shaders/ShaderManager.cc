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
    "in vec2 f_tex_offset;\n"
    "in vec4 f_fg_color;\n"
    "in vec4 f_bg_color;\n"
    "out vec4 fragment_color;\n"
    "uniform sampler2D font_texture;\n"
    "void main() {\n"
    "\tfragment_color = texture(font_texture, f_tex_offset) * f_fg_color;\n"
    "}\0"
  };

  const GLchar* glyph_vertex_shader = {
    "#version 330 core\n"
    "layout (location = 0) in vec2 vert_position;\n"
    "layout (location = 1) in vec2 inst_offset;\n"
    "layout (location = 2) in vec2 inst_scale;\n"
    "layout (location = 3) in vec2 tex_offset;\n"
    "layout (location = 4) in vec4 fg_color;\n"
    "layout (location = 5) in vec4 bg_color;\n"
    "out vec2 f_tex_offset;\n"
    "out vec4 f_fg_color;\n"
    "out vec4 f_bg_color;\n"
    "void main() {\n"
    "\tf_fg_color = fg_color;\n"
    "\tf_bg_color = bg_color;\n"
    "\tgl_Position = vec4((vert_position * inst_scale) + inst_offset, 0.0f, 1.0f);\n"
    "}\0"
  };
}
