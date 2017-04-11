/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/profilers/MonitorProfiler.hpp>

#include <unicorn/window_manager/Hub.hpp>

#include <unicorn/utility/Logger.hpp>

#include <vector>
#include <cmath>

namespace unicorn
{
namespace WindowManager
{

MonitorProfiler::MonitorProfiler(Hub& hub)
    : m_hub( hub )
{
    LOG_DEBUG("MonitorProfiler created");

    m_hub.MonitorCreated.connect(this, &MonitorProfiler::OnMonitorCreated);
}

MonitorProfiler::~MonitorProfiler()
{
    LOG_DEBUG("MonitorProfiler destroyed");
}

void MonitorProfiler::OnMonitorCreated(Monitor* const& pMonitor)
{
    pMonitor->StateChanged.connect(this, &MonitorProfiler::OnMonitorStateChanged);

    const uint32_t id = pMonitor->GetId();
    const std::pair<int32_t, int32_t>& physicalSize = pMonitor->GetPhysicalSize();
    const std::pair<int32_t, int32_t>& virtualPosition = pMonitor->GetVirtualPosition();

    LOG_DEBUG("Monitor[%d]: created:"
        "\r\n\t%16s\t%s"
        "\r\n\t%16s\t%dx%d"
        "\r\n\t%16s\t%d:%d"
        "\r\n\t%16s\t%s"
        "\r\n\t%16s\t%s"
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
        LOG_DEBUG("Monitor[%d] video mode[%*d]%s: %dx%d@%dHz R%d G%d B%d"
            , id, counterChars, i, (currentVideoMode == mode) ? "*" : " "
            , mode.width, mode.height
            , mode.refreshRate
            , mode.rgbBitDepth[0], mode.rgbBitDepth[1], mode.rgbBitDepth[2]
        );
    }
}

void MonitorProfiler::OnMonitorStateChanged(Monitor* pMonitor, Monitor::State state)
{
    LOG_DEBUG("Monitor[%d]: connected state changed to %s",
        pMonitor->GetId(),
        pMonitor->GetState() != MonitorMemento::State::Unknown ?
            (pMonitor->GetState() == MonitorMemento::State::Connected ?
                "true" : "false") : "unknown"
    );
}

}
}
