/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_MONITOR_MEMENTO_HPP
#define UNICORN_SYSTEM_MONITOR_MEMENTO_HPP

#include <unicorn/system/GammaRamp.hpp>
#include <unicorn/system/VideoMode.hpp>

#include <cstdint>
#include <utility>
#include <string>
#include <vector>

namespace unicorn
{
namespace system
{

/** @brief  Class describing monitor specifics */
struct MonitorMemento
{
    /** @brief  Monitor states */
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

    //! Supported video modes
    std::vector<VideoMode> modes;

    //! Flag describing if this monitor is primary
    bool isPrimary;

    //! Current state of monitor
    State state;

    //! Handle provided by window manager adapter
    void* handle;
};

}
}

#endif /* UNICORN_SYSTEM_MONITOR_MEMENTO_HPP */
