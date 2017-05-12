/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_TIMER_HPP
#define UNICORN_SYSTEM_TIMER_HPP

#include <chrono>
#include <unicorn/utility/SharedMacros.hpp>

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
     * \brief Constructs Timer object.
     * \param run If true, timer will reset in constuctor.
     */
    UNICORN_EXPORT Timer(bool run = false);
    /**
     * \brief Reset timer.
     */
    UNICORN_EXPORT void Reset();
    /**
     * \brief Return time in milliseconds
     * \return Time in milliseconds
     */
    UNICORN_EXPORT Milliseconds ElapsedMilliseconds() const;
    /**
    * \brief Return time in seconds
    * \return Time in seconds
    */
    UNICORN_EXPORT Seconds ElapsedSeconds() const;
    /**
    * \brief Return time in minutes
    * \return Time in minutes
    */
    UNICORN_EXPORT Minutes ElapsedMinutes() const;

private:
    HighResolutionClock::time_point m_start;
};
}
}

#endif // UNICORN_SYSTEM_TIMER_HPP
