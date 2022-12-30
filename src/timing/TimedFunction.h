// @author James Holtom

#ifndef TIMED_FUNCTION_H
#define TIMED_FUNCTION_H

#include <memory>
#include <vector>
#include "Timer.h"
#include "../utility/SolUtils.h"

namespace term_engine::timing {
  class TimedFunction;

  /// @brief Smart pointer to a timed function.
  typedef std::shared_ptr<TimedFunction> TimedFunctionPtr;
  /// @brief Used to store a list of timed functions.
  typedef std::vector<TimedFunctionPtr> TimedFunctionList;

  /// @brief Used to delay the execution of a function by a set amount of time.
  class TimedFunction {
  public:
    /**
     * @brief Constructs the timed function with the given parameters.
     * 
     * @param[in] delay    The amount of time to delay calling the function for, in milliseconds (ms).
     * @param[in] repeat   Whether to continuously call the function, using the delay as an interval.
     * @param[in] callback The function to call when the delay has been met.
     */
    TimedFunction(const int& delay, const bool& repeat, const sol::function callback);

    /**
     * @brief Returns if the timed function is active.
     * 
     * @returns If the timed function is active.
     */
    bool IsActive() const;

    /**
     * @brief Returns the amount of delay before calling the function, in milliseconds (ms).
     * 
     * @returns The amount of delay.
     */
    int GetDelay() const;

    /**
     * @brief Returns if the timed function repeats after being triggered.
     * 
     * @returns If the timed function repeats after being triggered.
     */
    bool IsRepeatable() const;

    /**
     * @brief Returns the number of times the function has been called.
     * 
     * @returns The number of times the function has been called.
     */
    int GetTimesRepeated() const;

    /**
     * @brief Sets if the function is active.
     * 
     * @param[in] flag The value to set.
     */
    void SetActive(const bool& flag);

    /**
     * @brief Returns if 2 timed functions share the same ID.
     * 
     * @param[in] rhs The timed function to compare this function to.
     * @returns If the 2 timed functions are the same.
     */
    bool operator==(const TimedFunction& rhs) const
    {
      return timed_id_ == rhs.timed_id_;
    }

    static void Update();

    static TimedFunctionPtr& Add(const int& delay, const bool& repeat, const sol::function callback);

    static TimedFunctionPtr& AddSelf(sol::object self, const int& delay, const bool& repeat, const sol::function callback);

    static void Remove(TimedFunctionPtr& timed_function);

    static int Count();

    /// @brief Removes all timed functions in the list.
    static void CleanUp();

  protected:
    /// @brief The ID of the timed function.
    int timed_id_;
    /// @brief Is the timed function active?
    bool active_;

    int delay_;

    bool repeat_;

    Timer timer_;

    int times_repeated_;
    /// @brief The callback function to call when the delay has finished.
    sol::function callback_;
    /// @brief Represents the ID to use for the next timed function.
    static int timed_next_id_;
    /// @brief The list of timed functions.
    static TimedFunctionList function_list_;
  };
}

#endif // ! TIMED_FUNCTION_H