/// @author James Holtom

#ifndef TIMED_FUNCTION_H
#define TIMED_FUNCTION_H

#include <string>
#include "BaseObject.h"
#include "../Timer.h"
#include "../../utility/SolUtils.h"

namespace term_engine::usertypes {
  /// @brief The type name for TimedFunctions.
  constexpr char TIMED_FUNCTION_TYPE[] = "TimedFunction";

  /// @brief Used to delay the execution of a function by a set amount of time.
  class TimedFunction : public BaseObject {
  public:
    /**
     * @brief Constructs the timed function with the given parameters.
     * 
     * @param[in] delay    The amount of time to delay calling the function for, in milliseconds (ms).
     * @param[in] start    Whether to start the timer upon creating the function.
     * @param[in] repeat   Whether to continuously call the function, using the delay as an interval.
     * @param[in] callback The function to call when the delay has been met.
     */
    TimedFunction(uint64_t delay, bool start, bool repeat, const sol::function callback);

    /// @brief Destroys the timed function.
    ~TimedFunction();

    /// @brief Updates the timed function.
    void Update(uint64_t timestep);

    /**
     * @brief Returns the type of the object.
     * 
     * @returns The object type.
     */
    std::string GetObjectType() const;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    ObjectSortPriority GetListPriority() const;

    /**
     * @brief Returns the amount of delay before calling the function, in milliseconds (ms).
     * 
     * @returns The amount of delay.
     */
    uint64_t GetDelay() const;

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
    uint32_t GetTimesCalled() const;

    void Start();

    void Stop();

    void SetRepeatable(bool flag);

    /// @brief Updates the debugging information for this object.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The amount of time since starting the timer.
    uint64_t accumulator_;
    /// @brief The length of the delay before triggering the callback.
    uint64_t delay_;
    /// @brief Whether the callback is triggered repeatedly or not.
    bool repeat_;
    /// @brief The amount of times the callback has been triggered.
    uint32_t times_called_;
    /// @brief The callback function to call when the delay has finished.
    sol::function callback_;
  };

  /**
   * @brief Adds a timed function to the list.
   * 
   * @param[in] delay    The amount of time to delay calling the function for, in milliseconds (ms).
   * @param[in] repeat   Whether to continuously call the function, using the delay as an interval.
   * @param[in] callback The function to call when the delay has been met.
   * @returns A raw pointer to the object.
   */
  TimedFunction* AddTimedFunction(uint64_t delay, bool started, bool repeat, const sol::function callback);

  /// @brief Clears all timed functions from the object list.
  void ClearAllTimedFunctions();
}

#endif // ! TIMED_FUNCTION_H
