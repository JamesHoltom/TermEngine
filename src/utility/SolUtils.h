/// @author James Holtom

#ifndef SOL_UTILS_H
#define SOL_UTILS_H

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace term_engine::utility {
  /**
   * @brief Calls the given Lua function with exception handling.
   * 
   * @param[in] func The Lua function to call.
   */
  void Call(sol::protected_function func);

  /**
   * @brief Calls the given Lua function, passing the timestep and handling exceptions.
   * 
   * @param[in] func The Lua function to call.
   * @param timestep The timestep.
   */
  void CallWithTimestep(sol::protected_function func, float timestep);

  /**
   * @brief Calls the given Lua function with exception handling.
   * 
   * @param[in] func The Lua function to call.
   * @returns The result of calling the function.
   */
  bool CallWithResult(sol::protected_function func);
}

#endif // ! SOL_UTILS_H
