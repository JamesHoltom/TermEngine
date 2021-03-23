/// @author James Holtom

#ifndef INIT_H
#define INIT_H

namespace term_engine::system {
  /// Initialises all sub-systems. Will exit the program if something fails.
  void Init();

  /// Cleans up all sub-systems.
  void CleanUp();
}

#endif // ! INIT_H