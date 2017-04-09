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

/** @brief  Class that reports all monitor events */
class MonitorProfiler
{
public:
    /** @brief  Constructs a monitor profiler */
    MonitorProfiler(Hub& hub);

    /** @brief  Deconstructs a monitor profiler */
    ~MonitorProfiler();

private:
    //! WindowManager::Hub reference
    Hub& m_hub;

    /** @brief  Slot invoked when monitor object is created */
    void OnMonitorCreated(Monitor* const& pMonitor);

    /** @brief  Slot invoked when monitor state is changed (connected/disconnected) */
    void OnMonitorStateChanged(Monitor* pMonitor, Monitor::State state);
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_PROFILERS_MONITOR_PROFILER_HPP */
