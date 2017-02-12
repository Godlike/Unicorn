/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_TIMER_HPP
#define UNICORN_SYSTEM_TIMER_HPP

#include <chrono>

namespace uc
{
namespace system
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

#endif // UNICORN_SYSTEM_TIMER_HPP
