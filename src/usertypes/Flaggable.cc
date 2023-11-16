#include "Flaggable.h"

namespace term_engine::usertypes {
  Flaggable::Flaggable() :
    marked_for_removal_(false)
  {}

  bool Flaggable::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void Flaggable::FlagForRemoval()
  {
    marked_for_removal_ = true;
  }

  void Flaggable::UnflagForRemoval()
  {
    marked_for_removal_ = false;
  }
}
