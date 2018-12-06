/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_UTILITY_SETTINGS_HPP
#define UNICORN_UTILITY_SETTINGS_HPP

#include <mule/templates/Singleton.hpp>

#include <string>
#include <cstdint>

namespace unicorn
{
namespace utility
{

/** @brief  Provides an access to application settings */
class Settings : public mule::templates::Singleton<Settings>
{
public:
    /** @brief Bit mask class describing profiling settings */
    struct ProfilingMask
    {
        typedef uint16_t MaskType;

        static const MaskType None      = 0;

        static const MaskType Window    = 1 << 0;
        static const MaskType Monitor   = 1 << 1;
        static const MaskType Mouse     = 1 << 2;
        static const MaskType Key       = 1 << 3;
        static const MaskType Gamepad   = 1 << 4;
    };

    //! Returns application width
    uint32_t GetApplicationWidth() const { return m_width; }

    //! Returns application height
    uint32_t GetApplicationHeight() const { return m_height; }

    /** @brief  Sets application width
     *
     *  @param  width   new width
     */
    void SetApplicationWidth(uint32_t width) { m_width = width; }

    /** @brief  Sets application height
     *
     *  @param  height   new height
     */
    void SetApplicationHeight(uint32_t height) { m_height = height; }

    //! Returns application name
    const std::string& GetApplicationName() const { return m_applicationName; }

    //! Returns engine name
    const std::string& GetUnicornEngineName() const { return m_unicornEngineName; }

    /** @brief  Sets an application name
     *
     *  @param  name new name
     */
    void SetApplicationName(const std::string&& name) { m_applicationName = name; }

    /** @brief  Sets an engine name
     *
     *  @param  name new name
     */
    void SetUnicornEngineName(const std::string&& name) { m_unicornEngineName = name; }

    //! Returns profiling mask
    ProfilingMask::MaskType GetProfilingMask() const { return m_profilingMask; }

    /** @brief  Sets profiling mask
     *
     *  @param  profilingMask   new profiling mask
     */
    void SetProfilingMask(ProfilingMask::MaskType profilingMask) { m_profilingMask = profilingMask; }

private:
    friend class mule::templates::Singleton<Settings>;

    /** @brief  Constructs an object with default settings */
    Settings();
    ~Settings() = default;

    Settings(const Settings& other) = delete;
    Settings& operator=(const Settings& other) = delete;

    Settings(Settings&& other) = delete;
    Settings& operator=(Settings&& other) = delete;

    //! Application width
    uint32_t m_width;

    //! Application height
    uint32_t m_height;

    //! Application name
    std::string m_applicationName;

    //! Engine name
    std::string m_unicornEngineName;

    //! Profiling mask
    ProfilingMask::MaskType m_profilingMask;
};
}
}

#endif // UNICORN_UTILITY_SETTINGS_HPP
