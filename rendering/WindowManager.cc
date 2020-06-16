#include <spdlog/spdlog.h>

#include "ShaderManager.h"
#include "WindowManager.h"

namespace term_engine::window_management {
  void InitDefaultWindow() {
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
    GLuint program_id;
    GLint compile_vertex_ok = GL_FALSE;
    GLint compile_fragment_ok = GL_FALSE;
    GLint link_program_ok = GL_FALSE;

    WindowPtr window = AddWindow("default");

    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    std::tie(compile_vertex_ok, vertex_shader_id) = shaders::BuildShaderFromString(shaders::default_vertex_shader, GL_VERTEX_SHADER);
    std::tie(compile_fragment_ok, fragment_shader_id) = shaders::BuildShaderFromString(shaders::default_fragment_shader, GL_FRAGMENT_SHADER);

    if (compile_vertex_ok == GL_TRUE && compile_fragment_ok == GL_TRUE) {
      std::tie(link_program_ok, program_id) = shaders::BuildProgram("default", { vertex_shader_id, fragment_shader_id });

      if (link_program_ok == GL_TRUE) {
        window->SetProgram(program_id);

        spdlog::info("Built shader program with ID {}.", program_id);
      }
    }
  }

  void CleanUpWindows() {
    window_list.clear();
  }

  WindowPtr GetDefaultWindow() {
    return window_list.at("default");
  }

  WindowPtr GetWindow(const std::string& name) {
    return window_list.at(name);
  }

  int GetWindowCount() {
    return window_list.size();
  }

  WindowPtr AddWindow(const std::string& name) {
    auto window = window_list.insert(std::make_pair(name, std::make_shared<Window>(name)));

    return window.first->second;
  }

  void RemoveWindow(const std::string& name) {
    window_list.erase(name);
  }

  void Logic(SDL_Event& event) {
    for (auto window = window_list.begin(); window != window_list.end();) {
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == window->second->GetID()) {
        spdlog::info("Destroying window {}", window->first);

        window = window_list.erase(window);
      }
      else {
        window->second->Logic(event);

        window++;
      }
    }
  }

  void Render() {
    for (WindowPair window : window_list) {
      window.second->Render();
    }
  }

  WindowMap window_list;
}