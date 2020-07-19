#include <spdlog/spdlog.h>

#include "../shaders/ShaderManager.h"
#include "../text/GlyphManager.h"
#include "WindowManager.h"

namespace term_engine::windows {
  void InitDefaultWindow() {
    WindowPtr window = AddWindow("default");

    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
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

  size_t GetWindowCount() {
    return window_list.size();
  }

  WindowPtr AddWindow(const std::string& name) {
    auto window = window_list.emplace(std::make_pair(name, std::make_shared<Window>(name)));

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