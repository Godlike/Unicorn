/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_PROFILERS_MONITOR_PROFILER_HPP
#define UNICORN_WINDOW_MANAGER_PROFILERS_MONITOR_PROFILER_HPP

#include <unicorn/window_manager/Monitor.hpp>

namespace unicorn
{
namespace WindowManager
{

class Hub;

class MonitorProfiler
{
public:
    MonitorProfiler(Hub& hub);
    ~MonitorProfiler();

private:
    Hub& m_hub;

    void OnMonitorCreated(Monitor* const& pMonitor);
    void OnMonitorStateChanged(Monitor* pMonitor, Monitor::State state);
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_PROFILERS_MONITOR_PROFILER_HPP */
