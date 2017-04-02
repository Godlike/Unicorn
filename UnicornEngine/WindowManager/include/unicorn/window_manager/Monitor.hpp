/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_MONITOR_HPP
#define UNICORN_WINDOW_MANAGER_MONITOR_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Provides information about monitor
 *
 *  Proxies calls to window manager adapter
 */
class Monitor
{
public:
    /** @brief  Represents monitor's video modes */
    struct VideoMode
    {
        int32_t width;
        int32_t height;
        int32_t redBits;
        int32_t greenBits;
        int32_t blueBits;
        int32_t refreshRate;
    };

    /** @brief  Constructs a monitor object */
    Monitor();

    /** @brief  Returns the id of the monitor */
    uint32_t GetId() { return m_id; }

    /** @brief  Returns the name of the monitor */
    const std::string& GetName() const { return m_name; }

    /** @grief  Returns a handle provided by window manager adapter */
    void* GetHandle() { return m_handle; }

private:
    //! Monitor id within application
    uint32_t m_id;

    //! Human readable name
    std::string m_name;

    //! Physical width
    uint32_t m_width;

    //! Physical height
    uint32_t m_height;

    //! Virtual position on X axis
    int32_t m_x;

    //! Virtual position on Y axis
    int32_t m_y;

    //! Vector of supported @ref VideoMode objects
    std::vector<VideoMode> m_modes;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_WINDOW_MANAGER_MONITOR_HPP
