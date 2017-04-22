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
class Timer
{
    using HighResolutionClock = std::chrono::high_resolution_clock;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
public:
    UNICORN_EXPORT Timer(bool run = false);
    UNICORN_EXPORT void Start();
    UNICORN_EXPORT Milliseconds ElapsedMilliseconds() const;
    UNICORN_EXPORT Seconds ElapsedSeconds() const;
    UNICORN_EXPORT Minutes ElapsedMinutes() const;
private:
    HighResolutionClock::time_point m_start;
};
}
}

#endif // UNICORN_SYSTEM_TIMER_HPP
