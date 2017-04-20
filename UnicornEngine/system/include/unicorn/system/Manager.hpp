/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_MANAGER_HPP
#define UNICORN_SYSTEM_MANAGER_HPP

#include <unicorn/system/WindowHint.hpp>
#include <unicorn/system/MonitorMemento.hpp>

#include <unicorn/system/input/GamepadState.hpp>

#include <wink/signal.hpp>
#include <wink/event_queue.hpp>

#include <vulkan/vulkan.hpp>

#include <map>
#include <vector>

namespace unicorn
{
namespace system
{

namespace input
{
class Gamepad;
}

class Monitor;
class Window;

/** @brief  Provides interface to managing windows and monitors
 *
 *  Proxies calls to window manager adapter
 */
class Manager
{
public:
    /** @brief  Constructs a system manager object */
    Manager();

    Manager(const Manager& other) = delete;
    Manager& operator=(const Manager& other) = delete;

    Manager(Manager&& other) = delete;
    Manager& operator=(Manager&& other) = delete;

    /** @brief  Deconstructs a system manager object
     *
     *  Also calls Deinit()
     *
     *  @sa Deinit()
     */
    ~Manager();

    /** @brief  Creates a window object
     *
     *  @param  width           desired width
     *  @param  height          desired height
     *  @param  name            desired name
     *  @param  pMonitor        pointer to a monitor where window
     *                          should be created
     *  @param  pSharedWindow   pointer to another window that should
     *                          share all resources with created window
     *
     *  @return a pointer to newly created window object
     *
     *  @sa DestroyWindow()
     */
    Window* CreateWindow(int32_t width,
        int32_t height,
        const std::string& name,
        Monitor* pMonitor = nullptr,
        Window* pSharedWindow = nullptr);

    /** @brief  Creates a vulkan surface that can be used with this window
     *
     *  @param[in]  window      window which should be associated with surface
     *  @param[in]  instance    Vulkan instance
     *  @param[in]  allocator   Vulkan allocator
     *  @param[out] surface     variable that will hold created surface handle
     *
     *  @return Vulkan result flag
     */
    VkResult CreateVulkanSurfaceForWindow(const Window& window,
        VkInstance instance,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface);

    /** @brief  Returns a window identified by @p id
     *
     *  @param  id  window id
     *
     *  @return a pointer to window object or @c nullptr if the window
     *          was not found
     */
    Window* GetWindow(uint32_t id) const;

    /** @brief  Returns a pointer to currently focused window
     *
     *  @return pointer to currently focused window or @c nullptr if there is
     *          no focused window controlled by the program
     */
    Window* GetFocusedWindow() const;

    /** @brief  Destroys a window identified by @p id
     *
     *  Searches for the window by calling GetWindow() and then calls
     *  DestroyWindow(Window* pWindow) to destroy the window.
     *
     *  @param  id  window id
     *
     *  @return @c true if the window was destroyed, @c false otherwise
     *
     *  @sa DestroyWindow(Window* pWindow)
     */
    bool DestroyWindow(uint32_t id);

    /** @brief  Tries to destroy given window
     *
     *  Searches for the window in @p m_windows and destroys it if it is found,
     *  otherwise does nothing and returns @c false
     *
     *  @param  pWindow pointer to a window to be detroyed
     *
     *  @return @c true if the window was destroyed, @c false otherwise
     *
     *  @sa DestroyWindow(uint32_t id)
     */
    bool DestroyWindow(Window* pWindow);

    /** @brief  Initializes window managing subsystem
     *
     *  Requests monitor information from window manager adapter
     *  and subscribes to his events
     */
    void Init();

    /** @brief  Deinitializes window managing subsystem
     *
     *  Unsubscribes from window manager adapret events and destroys
     *  all created windows and monitors
     */
    void Deinit();

    /** @brief  Polls for window and monitor events */
    void PollEvents() const;

    /** @brief  Checks whether window managing subsystem supports Vulkan
     *
     *  @return @c true if Vulkan is supported, @c false otherwise
     */
    bool IsVulkanSupported() const;

    /** @brief  Returns a vector of required Vulkan extensions
     *
     *  Checks which Vulkan extensions are required for proper
     *  window management subsystem functionality.
     *
     *  @return a vector of Vulkan extension names
     */
    std::vector<const char*> GetRequiredVulkanExtensions() const;

    /** @brief  Returns a monitor identified by id
     *
     *  @param  id  monitor id
     *
     *  @return a pointer to monitor object or @c nullptr if the monitor
     *          was not found
     */
    Monitor* GetMonitor(uint32_t id) const;

    /** @brief  Returns a vector of all known monitors */
    const std::vector<Monitor*>& GetMonitors() const { return m_monitors; }

    /** @brief  Returns monitor associated with given @p window
     *
     *  @param  window  window object
     *
     *  @return a pointer to monitor object associated with given @p window
     *          or @c nullptr if something went wrong or the window is not in
     *          fullscreen mode
     */
    Monitor* GetWindowMonitor(const Window& window) const;

    /** @brief  Sets monitor to be used by @p window for fullscreen mode
     *
     *  @param  window      window object
     *  @param  pMonitor    monitor object or @c nullptr for windowed mode
     *  @param  position    desired position as (x, y)
     *  @param  size        desired size as (width, height)
     *  @param  refreshRate desired refresh rate
     */
    void SetWindowMonitor(const Window& window,
        Monitor* pMonitor,
        std::pair<int32_t, int32_t> position,
        std::pair<int32_t, int32_t> size,
        int32_t refreshRate) const;

    /** @brief  Sets window creation hints
     *
     *  @param  hint    window creation hint
     *  @param  value   new value
     */
    void SetWindowCreationHint(WindowHint hint, int32_t value) const;

    /** @brief  Returns all connected gamepads */
    const std::map<uint32_t, input::Gamepad*>& GetGamepads() const { return m_gamepads; }

    /** @brief  Polls all connected gamepads for new values */
    void PollGamepads();

    /** @brief  Event triggered every time new window is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created window pointer
     */
    wink::signal< wink::slot<void(Window*)> > WindowCreated;

    /** @brief  Event queue triggered every time new batch of monitors is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created monitor pointer
     */
    wink::event_queue<Monitor*> MonitorCreated;

    /** @brief  Event queue triggered every time new bath of gamepads is created
     *
     *  Event is emitted with the following signature:
     *  -# newly created gamepad
     */
    wink::event_queue<input::Gamepad*> GamepadCreated;

private:
    /** @brief  Slot invoked when monitor state is changed (connected/disconnected) */
    void OnMonitorStateChanged(void* handle, MonitorMemento::State state);

    /** @brief  Slot invoked when gamepad state is changed (connected/disconnected) */
    void OnGamepadStateChanged(void* handle, input::GamepadState state);

    //! Returns a monitor identified by handle
    Monitor* GetMonitor(void* handle) const;

    //! A map of @ref Window objects identified by their id
    std::map<uint32_t, Window*> m_windows;

    //! A map of @ref input::Gamepad objects identified by their id
    std::map<uint32_t, input::Gamepad*> m_gamepads;

    //! A vector of available @ref Monitor objects
    std::vector<Monitor*> m_monitors;

};

}
}

#endif // UNICORN_SYSTEM_MANAGER_HPP
