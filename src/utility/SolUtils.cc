#include "SolUtils.h"
#include "LogUtils.h"

namespace term_engine::utility {
  void Call(sol::protected_function func)
  {
    if (!func.valid())
    {
      return;
    }

    try {
      sol::protected_function_result result = func();

      if (!result.valid())
      {
        sol::error err = result;
        utility::LogError("Received Lua error!\nError: {}. ", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nError: {}", err.what());
    }
  }

  void CallWithTimestep(sol::protected_function func, float timestep)
  {
    if (!func.valid())
    {
      return;
    }

    try {
      sol::protected_function_result result = func(timestep);

      if (!result.valid())
      {
        sol::error err = result;
        utility::LogError("Received Lua error!\nError: {}. ", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nError: {}", err.what());
    }
  }

  bool CallWithResult(sol::protected_function func)
  {
    if (!func.valid())
    {
      return false;
    }

    bool return_value = false;
    
    try {
      sol::protected_function_result result = func();

      if (result.valid())
      {
        return_value = (bool)result;
      }
      else
      {
        sol::error err = result;
        utility::LogError("Failed to call function\nError: {}. ", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nError: {}", err.what());
    }

    return return_value;
  }
}