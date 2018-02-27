/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/profiler/MonitorProfiler.hpp>

#include <unicorn/system/Manager.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

#include <vector>
#include <cmath>

namespace unicorn
{
namespace system
{

MonitorProfiler::MonitorProfiler(Manager& manager)
    : m_systemManager( manager )
{
    LOG_PROFILER->Info("MonitorProfiler created");

    m_systemManager.MonitorCreated.connect(this, &MonitorProfiler::OnMonitorCreated);
}

MonitorProfiler::~MonitorProfiler()
{
    LOG_PROFILER->Info("MonitorProfiler destroyed");
}

void MonitorProfiler::OnMonitorCreated(Monitor* const& pMonitor)
{
    pMonitor->StateChanged.connect(this, &MonitorProfiler::OnMonitorStateChanged);

    const uint32_t id = pMonitor->GetId();
    const std::pair<int32_t, int32_t>& physicalSize = pMonitor->GetPhysicalSize();
    const std::pair<int32_t, int32_t>& virtualPosition = pMonitor->GetVirtualPosition();

    LOG_PROFILER->Info("Monitor[{}]: created:"
        "\r\n\t{:>16}\t{}"
        "\r\n\t{:>16}\t{}x{}"
        "\r\n\t{:>16}\t{}:{}"
        "\r\n\t{:>16}\t{}"
        "\r\n\t{:>16}\t{}"
        , id
        , "name", pMonitor->GetName().c_str()
        , "physical size", physicalSize.first, physicalSize.second
        , "virtual position", virtualPosition.first, virtualPosition.second
        , "primary monitor", pMonitor->IsPrimary() ? "true" : "false"
        , "connected", pMonitor->GetState() != MonitorMemento::State::Unknown ?
                        (pMonitor->GetState() == MonitorMemento::State::Connected ?
                            "true" : "false") : "unknown"
    );

    const VideoMode currentVideoMode = pMonitor->GetActiveVideoMode();
    const std::vector<VideoMode>& videoModes = pMonitor->GetVideoModes();
    const uint32_t counterChars = static_cast<uint32_t>(log10(videoModes.size())) + 1;
    for (uint32_t i = 0; i < videoModes.size(); ++i)
    {
        const VideoMode& mode = videoModes[i];
        LOG_PROFILER->Info("Monitor[{}] video mode[{:>{}}]{}: {}x{}@{}Hz R{} G{} B{}"
            , id, i, counterChars, (currentVideoMode == mode) ? "*" : " "
            , mode.width, mode.height
            , mode.refreshRate
            , mode.rgbBitDepth[0], mode.rgbBitDepth[1], mode.rgbBitDepth[2]
        );
    }
}

void MonitorProfiler::OnMonitorStateChanged(Monitor* pMonitor, Monitor::State state)
{
    LOG_PROFILER->Info("Monitor[{}]: connected state changed to {}",
        pMonitor->GetId(),
        pMonitor->GetState() != MonitorMemento::State::Unknown ?
            (pMonitor->GetState() == MonitorMemento::State::Connected ?
                "true" : "false") : "unknown"
    );
}

}
}
