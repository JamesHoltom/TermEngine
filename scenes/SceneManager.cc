#include "SceneManager.h"
#include "../logging/Logger.h"

namespace term_engine::scenes {
  ScenePtr GetScene(const std::string& name) {
    auto result = scene_list.find(name);

    if (result == scene_list.end()) {
      logging::logger->warn("Could not find scene \"{}\".", name);

      return nullptr;
    }
    else {
      return result->second;
    }
  }

  ScenePtr AddScene(const std::string& name) {
    ScenePtr new_scene = std::make_shared<Scene>();
    auto result = scene_list.emplace(std::make_pair(name, std::move(new_scene)));

    return result.first->second;
  }

  void RemoveScene(const std::string& name) {
    scene_list.erase(name);
  }

  void CleanUpScenes() {
    scenes::active_scene_.reset();
    scene_list.clear();
  }

  SceneList scene_list;
  ScenePtr active_scene_;
}
