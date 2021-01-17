/// @author James Holtom

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

namespace term_engine::scenes {
  /// Stores a list of scenes.
  extern SceneList scene_list;
  /// The scene that is currently active in the program.
  extern ScenePtr active_scene_;

  /// Returns the scene with the given name.
  /**
   * @param[in] name The name of the scene to get.
   * @returns A pointer to the scene, or `nullptr` if it was not found.
   */
  ScenePtr GetScene(const std::string& name);

  /// Adds a new scene to the list.
  /**
   * @param[in] name The name of the new scene.
   * @returns A pointer to the newly created scene.
   */
  ScenePtr AddScene(const std::string& name);

  /// Removes a scene from the list.
  /**
   * @param[in] name The name of the scene to remove.
   */
  void RemoveScene(const std::string& name);

  /// Removes all scenes from the list.
  void CleanUpScenes();
}

#endif // ! SCENE_MANAGER_H
