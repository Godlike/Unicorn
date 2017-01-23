/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/core/Timer.hpp>

vp::core::Timer::Timer() {}

void vp::core::Timer::Start() { m_epoch = Clock::now(); }

vp::core::Timer::Duration vp::core::Timer::Elapsed() const {
  return Clock::now() - m_epoch;
}

long long vp::core::Timer::Milliseconds() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(Elapsed())
      .count();
}

long long vp::core::Timer::Hours() const {
  return std::chrono::duration_cast<std::chrono::hours>(Elapsed()).count();
}

long long vp::core::Timer::Microseconds() const {
  return std::chrono::duration_cast<std::chrono::microseconds>(Elapsed())
      .count();
}

long long vp::core::Timer::Minutes() const {
  return std::chrono::duration_cast<std::chrono::minutes>(Elapsed()).count();
}

long long vp::core::Timer::Nanoseconds() const {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(Elapsed())
      .count();
}

long long vp::core::Timer::Seconds() const {
  return std::chrono::duration_cast<std::chrono::seconds>(Elapsed()).count();
}
