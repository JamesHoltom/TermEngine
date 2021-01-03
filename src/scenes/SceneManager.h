/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

namespace term_engine::scenes {
  ScenePtr GetScene(const std::string& name);
  ScenePtr AddScene(const std::string& name);
  void RemoveScene(const std::string& name);

  void CleanUpScenes();

  extern SceneList scene_list;
  extern ScenePtr active_scene_;
}

#endif // ! SCENE_MANAGER_H
