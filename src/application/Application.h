/**
 * @author James Holtom
 */

#ifndef APPLICATION_H
#define APPLICATION_H

namespace term_engine::application {
  /**
   * Performs setup of the application and all its sub-systems.
   */
  void Init();

  /**
   * Performs clean-up of all resources used by the application.
   */
  void CleanUp();

  /**
   * Runs the application. This function will loop continuously until the 'quit' flag is set.
   */
  void Run();
}

#endif // ! APPLICATION_H
