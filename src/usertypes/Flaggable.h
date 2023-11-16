/// @author James Holtom
#ifndef FLAGGABLE_H
#define FLAGGABLE_H

namespace term_engine::usertypes {
  class Flaggable {
  public:
    /// @brief Constructs the flaggable object.
    Flaggable();

    /**
     * @brief Returns if the object is flagged to be removed.
     * 
     * @returns If the object is flagged to be removed.
     */
    bool FlaggedForRemoval() const;

    /// @brief Flags the object to be removed.
    void FlagForRemoval();

    /// @brief Unsets the removal flag from the object.
    void UnflagForRemoval();

  protected:
    /// @brief A flag to mark this object to be removed.
    bool marked_for_removal_;
  };
}

#endif // ! FLAGGABLE_H
