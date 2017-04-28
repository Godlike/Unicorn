/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Timer.hpp>

namespace unicorn
{
    namespace system
    {
        Timer::Timer(bool run)
        {
            if (run)
            {
                Reset();
            }
        }

        void Timer::Reset()
        {
            m_start = HighResolutionClock::now();
        }

        Timer::Milliseconds Timer::ElapsedMilliseconds() const
        {
            return std::chrono::duration_cast<Milliseconds>(HighResolutionClock::now() - m_start);
        }

        Timer::Seconds Timer::ElapsedSeconds() const
        {
            return std::chrono::duration_cast<Seconds>(HighResolutionClock::now() - m_start);
        }

        Timer::Minutes Timer::ElapsedMinutes() const
        {
            return std::chrono::duration_cast<Minutes>(HighResolutionClock::now() - m_start);
        }
    }
}