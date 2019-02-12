/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_TIMER_HPP
#define UNICORN_SYSTEM_TIMER_HPP

#include <chrono>

namespace unicorn
{
namespace system
{
/**
* @brief Timer class for time-related tasks.
*/
class Timer
{
    using HighResolutionClock = std::chrono::high_resolution_clock;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;

public:
    /**
     * @brief Constructs Timer object.
     * @param run If true, timer will reset in constuctor.
     */
    Timer(bool run = false);
    /**
     * @brief Resets timer.
     */
    void Reset();
    /**
     * @brief Returns time in milliseconds
     * @return Time in milliseconds
     */
    Milliseconds ElapsedMilliseconds() const;
    /**
    * @brief Returns time in seconds
    * @return Time in seconds
    */
    Seconds ElapsedSeconds() const;
    /**
    * @brief Returns time in minutes
    * @return Time in minutes
    */
    Minutes ElapsedMinutes() const;

private:
    HighResolutionClock::time_point m_start;
};
}
}

#endif // UNICORN_SYSTEM_TIMER_HPP
