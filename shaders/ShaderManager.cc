#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "../utility/spdlogUtils.h"

namespace term_engine::shaders {
  void InitGlyphShader() {
    ShaderPtr shader = AddShader("glyph");

    bool fs_result = shader->BuildShaderFromString(glyph_fragment_shader, GL_FRAGMENT_SHADER);
    bool vs_result = shader->BuildShaderFromString(glyph_vertex_shader, GL_VERTEX_SHADER);
    bool prog_result = shader->BuildProgram();

    spdlog::debug("Initialising glyph shader.");
    spdlog::debug("Fragment shader: {}", fs_result);
    spdlog::debug("Vertex shader:   {}", vs_result);
    spdlog::debug("Program:         {}", prog_result);
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
    "#version 440 core\n"
    "in vec3 f_tex_offset;\n"
    "in vec4 f_fg_color;\n"
    "in vec4 f_bg_color;\n"
    "out vec4 fragment_color;\n"
    "uniform usampler2DArray font_texture;\n"
    "void main() {\n"
    "\tfloat sampled = texture(font_texture, f_tex_offset).r;\n"
    "\tfragment_color = mix((vec4(1.0f, 1.0f, 1.0f, 1.0f - sampled) * f_bg_color), (vec4(1.0f, 1.0f, 1.0f, sampled) * f_fg_color), sampled);\n"
    "}\0"
  };

  const GLchar* glyph_vertex_shader = {
    "#version 440 core\n"
    "layout (location = 0) in vec2 vert_position;\n"
    "layout (location = 1) in vec2 tex_position;\n"
    "layout (location = 2) in vec2 vert_offset;\n"
    "layout (location = 3) in float tex_offset;\n"
    "layout (location = 4) in vec4 fg_color;\n"
    "layout (location = 5) in vec4 bg_color;\n"
    "out vec3 f_tex_offset;\n"
    "out vec4 f_fg_color;\n"
    "out vec4 f_bg_color;\n"
    "uniform mat4 projection;\n"
    "uniform ivec2 scale;\n"
    "void main() {\n"
    "\tf_tex_offset = vec3(tex_position, tex_offset);\n"
    "\tf_fg_color = fg_color;\n"
    "\tf_bg_color = bg_color;\n"
    "\tgl_Position = projection * vec4((vert_position * scale) + vert_offset, 0.0f, 1.0f);\n"
    "}\0"
  };
}
