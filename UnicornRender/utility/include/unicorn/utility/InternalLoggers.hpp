/*
* Copyright (C) 2018 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_INTERNAL_LOGGERS_HPP
#define UNICORN_UTILITY_INTERNAL_LOGGERS_HPP

#include <mule/Logger.hpp>

#include <array>
#include <cstdint>

namespace unicorn
{

struct Log
{
    static constexpr uint32_t unicorn = 0;
    static constexpr uint32_t unicorn_profiler = 1;
    static constexpr uint32_t unicorn_input = 2;
    static constexpr uint32_t unicorn_video = 3;
    static constexpr uint32_t unicorn_vulkan = 4;

    static constexpr uint32_t size = 5;
};

extern std::array<mule::LoggerPtr, Log::size> g_loggers;

#define LOG ::unicorn::g_loggers[::unicorn::Log::unicorn]
#define LOG_PROFILER ::unicorn::g_loggers[::unicorn::Log::unicorn_profiler]
#define LOG_INPUT ::unicorn::g_loggers[::unicorn::Log::unicorn_input]
#define LOG_VIDEO ::unicorn::g_loggers[::unicorn::Log::unicorn_video]
#define LOG_VULKAN ::unicorn::g_loggers[::unicorn::Log::unicorn_vulkan]

}

#endif // UNICORN_UTILITY_INTERNAL_LOGGERS_HPP
