/// @author James Holtom

#ifndef TIMED_FUNCTION_H
#define TIMED_FUNCTION_H

#include <string>
#include "BaseObject.h"
#include "../timing/Timer.h"
#include "../utility/SolUtils.h"

namespace term_engine::objects {
  /// @brief Used to delay the execution of a function by a set amount of time.
  class TimedFunction : public BaseObject {
  public:
    /**
     * @brief Constructs the timed function with the given parameters.
     * 
     * @param[in] delay    The amount of time to delay calling the function for, in milliseconds (ms).
     * @param[in] repeat   Whether to continuously call the function, using the delay as an interval.
     * @param[in] callback The function to call when the delay has been met.
     */
    TimedFunction(const int& delay, const bool& repeat, const sol::function callback);

    /// @brief Destroys the timed function.
    ~TimedFunction();

    /// @brief Updates the timed function.
    void Update();

    /**
     * @brief Returns the type of the object.
     * 
     * @return The object type.
     */
    std::string GetObjectType() const;

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
     * @brief Adds a timed function to the list.
     * 
     * @param[in] delay    The amount of time to delay calling the function for, in milliseconds (ms).
     * @param[in] repeat   Whether to continuously call the function, using the delay as an interval.
     * @param[in] callback The function to call when the delay has been met.
     * @returns A smart pointer to the timed function if it was added to the list, or a null pointer if it failed.
     */
    static ObjectPtr& Add(const int& delay, const bool& repeat, const sol::function callback);

    /// @brief Clears all timed functions from the object list.
    static void ClearAll();

  protected:
    /// @brief The timer used to measure when to trigger the callback.
    timing::Timer timer_;
    /// @brief The length of the delay before triggering the callback.
    int delay_;
    /// @brief Whether the callback is triggered repeatedly or not.
    bool repeat_;
    /// @brief The amount of times the callback has been triggered.
    int times_repeated_;
    /// @brief The callback function to call when the delay has finished.
    sol::function callback_;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    ObjectSortPriority GetListPriority() const;
  };
}

#endif // ! TIMED_FUNCTION_H
