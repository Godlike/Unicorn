/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_MONITOR_MEMENTO_HPP
#define UNICORN_WINDOW_MANAGER_MONITOR_MEMENTO_HPP

#include <unicorn/window_manager/GammaRamp.hpp>
#include <unicorn/window_manager/VideoMode.hpp>

#include <cstdint>
#include <utility>
#include <string>
#include <vector>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Class describing monitor specifics */
struct MonitorMemento
{
    enum class State : uint8_t
    {
        Connected       = 0x00,
        Disconnected    = 0x01,

        Unknown         = 0xFF
    };

    //! Monitor id within application
    uint32_t id;

    //! Human-readable name
    std::string name;

    //! Physical size as (width, height)
    std::pair<int32_t, int32_t> physicalSize;

    //! Virtual position as (x, y)
    std::pair<int32_t, int32_t> virtualPosition;

    //! Supported video modes
    std::vector<VideoMode> modes;

    //! Gamma ramp
    GammaRamp gammaRamp;

    //! Flag describing if this monitor is primary
    bool isPrimary;

    //! Current state of monitor
    State state;

    //! Handle provided by window manager adapter
    void* handle;
};

}
}

#endif /* UNICORN_WINDOW_MANAGER_MONITOR_MEMENTO_HPP */
