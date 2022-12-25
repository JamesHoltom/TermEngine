/// @author James Holtom

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "Background.h"
#include "FontAtlas.h"
#include "Object.h"
#include "Window.h"
#include "../shaders/ShaderProgram.h"

namespace objects {
  class GameScene;

  typedef std::shared_ptr<GameScene> GameScenePtr;
  typedef std::unordered_map<std::string, GameScenePtr> GameSceneList;

  class GameScene {
  public:
    GameScene();
    ~GameScene();

    static GameScenePtr& Add(const std::string& name);

  private:
    std::string name_;
    Window window_;
    Background background_;
    ObjectList object_list_;
    shaders::ShaderProgramPtr shader_;

    static GameSceneList game_scene_list_;
  };
}

#endif // ! GAME_SCENE_H