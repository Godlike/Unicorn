/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_HUB_HPP
#define UNICORN_WINDOW_MANAGER_HUB_HPP

#include <unicorn/window_manager/Monitor.hpp>
#include <unicorn/window_manager/Window.hpp>

#include <wink/signal.hpp>

#include <map>
#include <vector>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Provides interface to managing windows and monitors
 *
 *  Proxies calls to window manager adapter
 */
class Hub
{
public:
    /** @brief  Constructs a hub object */
    Hub();

    Hub(const Hub& other) = delete;
    Hub& operator=(const Hub& other) = delete;

    Hub(Hub&& other) = delete;
    Hub& operator=(Hub&& other) = delete;

    /** @brief  Deconstructs a hub object
     *
     *  Also calls Deinit()
     *
     *  @sa Deinit()
     */
    ~Hub();

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

    /** @brief  Returns a window identified by @p id
     *
     *  @param  id  window id
     *
     *  @return a pointer to window object or @c nullptr if the window
     *          was not found
     */
    Window* GetWindow(uint32_t id) const;

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

    //! Signal that is emitted every time new window is created
    wink::signal< wink::slot<void(Window*)> > WindowCreated;

private:
    //! Returns a monitor identified by handle
    Monitor* GetMonitor(void* handle) const;

    //! A map of @ref Window objects identified by their id
    std::map<uint32_t, Window*> m_windows;

    //! A vector of available @ref Monitor objects
    std::vector<Monitor*> m_monitors;

};

}
}

#endif // UNICORN_WINDOW_MANAGER_HUB_HPP
