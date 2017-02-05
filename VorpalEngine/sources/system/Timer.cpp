/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/system/Timer.hpp>

vp::system::Timer::Timer()
{
}

void vp::system::Timer::Start()
{
    m_epoch = Clock::now();
}

vp::system::Timer::Duration vp::system::Timer::Elapsed() const
{
    return Clock::now() - m_epoch;
}

long long vp::system::Timer::Milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(Elapsed())
        .count();
}

long long vp::system::Timer::Hours() const
{
    return std::chrono::duration_cast<std::chrono::hours>(Elapsed()).count();
}

long long vp::system::Timer::Microseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(Elapsed())
        .count();
}

long long vp::system::Timer::Minutes() const
{
    return std::chrono::duration_cast<std::chrono::minutes>(Elapsed()).count();
}

long long vp::system::Timer::Nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(Elapsed())
        .count();
}

long long vp::system::Timer::Seconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(Elapsed()).count();
}
