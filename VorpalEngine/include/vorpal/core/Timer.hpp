/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_CORE_TIMER_HPP
#define VORPAL_CORE_TIMER_HPP

#include <chrono>

namespace vp
{
namespace core
{
class Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using Duration = Clock::duration;

   public:
    Timer();
    void Start();
    Duration Elapsed() const;
    long long Milliseconds() const;
    long long Hours() const;
    long long Microseconds() const;
    long long Minutes() const;
    long long Nanoseconds() const;
    long long Seconds() const;

   private:
    Clock::time_point m_epoch;
};
}
}

#endif  // VORPAL_CORE_TIMER_HPP
