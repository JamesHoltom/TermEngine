/// @author James Holtom

#ifndef APPLICATION_H
#define APPLICATION_H

#include "utility/LogUtils.h"

namespace term_engine {
  /// @brief Performs setup of the application and all its sub-systems.
  void Init();

  /// @brief Performs setup, ready for a new project.
  void InitProject();

  /// @brief Performs clean-up of all sub-systems used by the application.
  void CleanUp();

  /// @brief Performs clean-up of all project-related resources.
  void CleanUpProject();

  /// @brief Runs the application. This function will loop continuously until the 'quit' flag is set.
  void Run();
}

#endif // ! APPLICATION_H
