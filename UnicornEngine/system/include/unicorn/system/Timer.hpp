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
class Timer
{
    using HighResolutionClock = std::chrono::high_resolution_clock;
    using Milliseconds = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
public:
    explicit Timer(bool run = false);
    void Reset();
    Milliseconds ElapsedMilliseconds() const;
    Seconds ElapsedSeconds() const;
    Minutes ElapsedMinutes() const;
    template <typename T, typename Traits>
    friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const Timer& timer);
private:
    HighResolutionClock::time_point m_start;
};
}
}

#endif // UNICORN_SYSTEM_TIMER_HPP
