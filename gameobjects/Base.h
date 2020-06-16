#ifndef MOD_GAME_OBJECT_BASE_H
#define MOD_GAME_OBJECT_BASE_H

namespace term_engine::modules::game_objects {
  class BaseGameObject {
  public:
    virtual ~BaseGameObject() {}

    virtual int Update();

  protected:
    bool is_dirty_;
  };
}

#endif // !MOD_GAME_OBJECT_BASE_H
