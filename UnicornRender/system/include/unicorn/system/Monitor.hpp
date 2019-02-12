/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_MONITOR_HPP
#define UNICORN_SYSTEM_MONITOR_HPP

#include <unicorn/system/GammaRamp.hpp>
#include <unicorn/system/MonitorMemento.hpp>
#include <unicorn/system/VideoMode.hpp>

#include <wink/signal.hpp>

#include <cstdint>
#include <utility>
#include <string>
#include <vector>

namespace unicorn
{
namespace system
{

/** @brief  Provides information about monitor
 *
 *  Proxies calls to window manager adapter
 */
class Monitor
{
public:
    //! Alias to MonitorMemento::State
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
    uint32_t GetId() const { return m_id; }

    /** @brief  Returns the name of the monitor */
    const std::string& GetName() const { return m_name; }

    /** @brief  Returns the physical size of the monitor */
    const std::pair<int32_t, int32_t>& GetPhysicalSize() const { return m_physicalSize; }

    /** @brief  Returns the virtual position of the monitor */
    std::pair<int32_t, int32_t> GetVirtualPosition() const;

    /** @brief  Returns the vector of video modes supoported by this monitor */
    const std::vector<VideoMode>& GetVideoModes() const { return m_modes; }

    /** @brief  Returns video mode currently being used by this monitor */
    VideoMode GetActiveVideoMode() const;

    /** @brief  Returns the gamma ramp of the monitor */
    GammaRamp GetGammaRamp() const;

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
    bool IsPrimary() const;

    /** @brief  Returns current state of monitor */
    State GetState() const { return m_state; }

    /** @brief  Sets monitor state
     *
     *  @param  state   new monitor state
     */
    void SetState(State state) { m_state = state; }

    /** @brief  Returns a handle provided by window manager adapter */
    void* GetHandle() { return m_handle; }

    /** @brief  Event triggered when monitor state is changed (connected/disconnected)
     *
     *  Event is emitted with the following signature:
     *  -# monitor pointer
     *  -# monitor state
     */
    wink::signal< wink::slot<void(Monitor*, State)> > StateChanged;

private:
    /** @brief  Slot invoked when monitor state is changed (connected/disconnected)
     *
     *  Bound to Adapter::MonitorStateChanged
     *
     *  @param  handle  monitor handle
     *  @param  state   monitor state
     */
    void OnMonitorStateChanged(void* handle, MonitorMemento::State state);

    //! Monitor id within application
    uint32_t m_id;

    //! Human-readable name
    std::string m_name;

    //! Physical size as (width, height)
    std::pair<int32_t, int32_t> m_physicalSize;

    //! Vector of supported @ref VideoMode objects
    std::vector<VideoMode> m_modes;

    //! Current state of monitor
    State m_state;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_SYSTEM_MONITOR_HPP
