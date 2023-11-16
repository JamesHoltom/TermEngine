/// @author James Holtom

#ifndef SOL_UTILS_H
#define SOL_UTILS_H

/*
  * Sol currently has an issue regarding deprecation error C4996.
  * https://github.com/ThePhD/sol2/issues/945
  *
  * After MSVC 16.6.0, there is an issue building Sol.
  */
#pragma warning(push)
#pragma warning(disable: 4996)

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#pragma warning(pop)

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
