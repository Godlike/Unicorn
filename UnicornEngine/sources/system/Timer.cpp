/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Timer.hpp>

uc::system::Timer::Timer()
{
}

void uc::system::Timer::Start()
{
    m_epoch = Clock::now();
}

uc::system::Timer::Duration uc::system::Timer::Elapsed() const
{
    return Clock::now() - m_epoch;
}

long long uc::system::Timer::Milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(Elapsed())
        .count();
}

long long uc::system::Timer::Hours() const
{
    return std::chrono::duration_cast<std::chrono::hours>(Elapsed()).count();
}

long long uc::system::Timer::Microseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(Elapsed())
        .count();
}

long long uc::system::Timer::Minutes() const
{
    return std::chrono::duration_cast<std::chrono::minutes>(Elapsed()).count();
}

long long uc::system::Timer::Nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(Elapsed())
        .count();
}

long long uc::system::Timer::Seconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(Elapsed()).count();
}
