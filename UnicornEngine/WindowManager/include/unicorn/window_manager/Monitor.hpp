/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_MONITOR_HPP
#define UNICORN_WINDOW_MANAGER_MONITOR_HPP

#include <unicorn/window_manager/GammaRamp.hpp>
#include <unicorn/window_manager/MonitorMemento.hpp>
#include <unicorn/window_manager/VideoMode.hpp>

#include <wink/signal.hpp>

#include <cstdint>
#include <utility>
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
    typedef MonitorMemento::State State;

    /** @brief  Constructs a monitor object
     *
     *  @param  memento object describing monitor specifics
     */
    explicit Monitor(const MonitorMemento& memento);

    Monitor(const Monitor& other) = delete;
    Monitor& operator=(const Monitor& other) = delete;

    Monitor(Monitor&& other) = delete;
    Monitor& operator=(Monitor&& other) = delete;

    ~Monitor();

    /** @brief  Returns the id of the monitor */
    uint32_t GetId() { return m_id; }

    /** @brief  Returns the name of the monitor */
    const std::string& GetName() const { return m_name; }

    /** @brief  Returns the physical size of the monitor */
    const std::pair<int32_t, int32_t>& GetPhysicalSize() const { return m_physicalSize; }

    /** @brief  Returns the virtual position of the monitor */
    const std::pair<int32_t, int32_t>& GetVirtualPosition() const { return m_virtualPosition; }

    /** @brief  Returns the vector of video modes supoported by this monitor */
    const std::vector<VideoMode>& GetVideoModes() const { return m_modes; }

    /** @brief  Returns the gamma ramp of the monitor */
    const GammaRamp& GetGammaRamp() const { return m_gammaRamp; }

    /** @brief  Sets gamma ramp for the monitor
     *
     *  @param  gammaRamp   new gamma ramp to be set
     */
    void SetGammaRamp(const GammaRamp& gammaRamp);

    /** @brief  Sets gamma ramp for the monitor
     *
     *  @param  gammaRamp   new gamma ramp to be set
     */
    void SetGammaRamp(GammaRamp&& gammaRamp);

    /** @brief  Generates gamma ramp for given gamma
     *
     *  @param  gamma   desired exponent
     *
     *  @return @c true if gamma was set, @c false otherwise
     */
    bool SetGamma(float gamma);

    /** @brief  Returns @c true if the monitor is primary, @c false otherwise */
    bool IsPrimary() const { return m_isPrimary; }

    /** @brief  Returns current state of monitor */
    State GetState() const { return m_state; }

    /** @brief  Sets monitor state
     *
     *  @param  state   new monitor state
     */
    void SetState(State state) { m_state = state; }

    /** @grief  Returns a handle provided by window manager adapter */
    void* GetHandle() { return m_handle; }

    wink::signal< wink::slot<void(Monitor*, State)> > StateChanged;

private:
    void OnMonitorStateChanged(void* handle, MonitorMemento::State state);

    //! Monitor id within application
    uint32_t m_id;

    //! Human-readable name
    std::string m_name;

    //! Physical size as (width, height)
    std::pair<int32_t, int32_t> m_physicalSize;

    //! Virtual position as (x, y)
    std::pair<int32_t, int32_t> m_virtualPosition;

    //! Vector of supported @ref VideoMode objects
    std::vector<VideoMode> m_modes;

    //! Gamma ramp
    GammaRamp m_gammaRamp;

    //! Flag describing if monitor is primary
    bool m_isPrimary;

    //! Current state of monitor
    State m_state;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_WINDOW_MANAGER_MONITOR_HPP
