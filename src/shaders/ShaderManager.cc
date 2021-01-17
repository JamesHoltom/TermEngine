#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "../logging/Logger.h"
#include "../window/Window.h"

namespace term_engine::shaders {
  ShaderList shader_list;

  void InitGlyphShader() {
    ShaderPtr shader = AddShader("glyph");

    bool vs_result = shader->BuildStageFromFile("resources/shaders/core/glyph.vs", GL_VERTEX_SHADER);
    bool gs_result = shader->BuildStageFromFile("resources/shaders/core/glyph.gs", GL_GEOMETRY_SHADER);
    bool fs_result = shader->BuildStageFromFile("resources/shaders/core/glyph.fs", GL_FRAGMENT_SHADER);
    bool prog_result = shader->BuildProgram();

    logging::logger->debug("Initialising glyph shader.");
    logging::logger->debug("Vertex shader:   {}", vs_result);
    logging::logger->debug("Geometry shader: {}", gs_result);
    logging::logger->debug("Fragment shader: {}", fs_result);
    logging::logger->debug("Program:         {}", prog_result);
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

  void GetPointerUsage() {
    for (ShaderIter shader : shader_list) {
      logging::logger->info("Shader {} has {} refs.", shader.first, shader.second.use_count());
    }
  }
}
