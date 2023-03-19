/// @author James Holtom

#ifndef APPLICATION_H
#define APPLICATION_H

#include "utility/DebugUtils.h"

namespace term_engine {
  extern utility::ObjectDebugInfoPtr app_debug_info;

  /// @brief Performs setup of the application and all its sub-systems.
  void Init();

  /// @brief Performs clean-up of all resources used by the application.
  void CleanUp();

  /// @brief Runs the application. This function will loop continuously until the 'quit' flag is set.
  void Run();
}

#endif // ! APPLICATION_H
