/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/system/Monitor.hpp>

#include <unicorn/system/MonitorMemento.hpp>

#include <unicorn/system/adapter/Helper.hpp>

#include <cmath>

namespace unicorn
{
namespace system
{

Monitor::Monitor(const MonitorMemento& memento)
    : m_id( memento.id )
    , m_name( memento.name )
    , m_physicalSize( memento.physicalSize )
    , m_modes( memento.modes )
    , m_state( memento.state )
    , m_handle( memento.handle )
{
    WINDOW_MANAGER_ADAPTER::MonitorStateChanged.connect(this, &Monitor::OnMonitorStateChanged);
}

Monitor::~Monitor()
{
    StateChanged.clear();
}

VideoMode Monitor::GetActiveVideoMode() const
{
    return WINDOW_MANAGER_ADAPTER::GetActiveVideoMode(m_handle);
}

std::pair<int32_t, int32_t> Monitor::GetVirtualPosition() const
{
    return WINDOW_MANAGER_ADAPTER::GetVirtualPosition(m_handle);
}

GammaRamp Monitor::GetGammaRamp() const
{
    return WINDOW_MANAGER_ADAPTER::GetGammaRamp(m_handle);
}

void Monitor::SetGammaRamp(const GammaRamp& gammaRamp)
{
    WINDOW_MANAGER_ADAPTER::SetGammaRamp(m_handle, gammaRamp);
}

void Monitor::SetGammaRamp(GammaRamp&& gammaRamp)
{
    WINDOW_MANAGER_ADAPTER::SetGammaRamp(m_handle, gammaRamp);
}

bool Monitor::SetGamma(float gamma)
{
    if (gamma <= 0.0f)
    {
        return false;
    }

    GammaRamp ramp;

    ramp.size = 256;

    uint16_t* values = new uint16_t[ramp.size];

    ramp.red = values;
    ramp.green = values;
    ramp.blue = values;

    float value = 0.0f;

    // Implementation copied over from glfw
    for (uint32_t i = 0; i < ramp.size; ++i)
    {
        // Calculate intensity
        value = i / 255.0f;

        // Apply gamma curve
        value = pow(value, 1.0f / gamma) * 65535.0f + 0.5f;

        // Clamp to value range
        if (value > 65535.0f)
        {
            value = 65535.0f;
        }

        values[i] = static_cast<uint16_t>(value);
    }

    SetGammaRamp(std::move(ramp));

    return true;
}

bool Monitor::IsPrimary() const
{
    return WINDOW_MANAGER_ADAPTER::GetPrimaryMonitor() == m_handle;
}

void Monitor::OnMonitorStateChanged(void* handle, MonitorMemento::State state)
{
    if (handle == m_handle)
    {
        m_state = state;

        StateChanged.emit(this, state);
    }
}

}
}
