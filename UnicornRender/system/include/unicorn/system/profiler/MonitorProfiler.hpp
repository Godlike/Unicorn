/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_PROFILER_MONITOR_PROFILER_HPP
#define UNICORN_SYSTEM_PROFILER_MONITOR_PROFILER_HPP

#include <unicorn/system/Monitor.hpp>

namespace unicorn
{
namespace system
{

class Manager;

/** @brief  Class that reports all monitor events */
class MonitorProfiler
{
public:
    /** @brief  Constructs a monitor profiler */
    MonitorProfiler(Manager& manager);

    /** @brief  Deconstructs a monitor profiler */
    ~MonitorProfiler();

private:
    //! system::Manager reference
    Manager& m_systemManager;

    /** @brief  Slot invoked when monitor object is created */
    void OnMonitorCreated(Monitor* const& pMonitor);

    /** @brief  Slot invoked when monitor state is changed (connected/disconnected) */
    void OnMonitorStateChanged(Monitor* pMonitor, Monitor::State state);
};

}
}

#endif /* UNICORN_SYSTEM_PROFILER_MONITOR_PROFILER_HPP */
