/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_WINDOW_HPP
#define UNICORN_WINDOW_MANAGER_WINDOW_HPP

#include <unicorn/window_manager/Monitor.hpp>

#include <wink/signal.hpp>

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <string>
#include <utility>

namespace unicorn
{
namespace WindowManager
{

/** @brief  Provides interface to system window
 *
 *  Proxies calls to window manager adapter
 */
class Window
{
public:
    /** @brief  Constructs a window object
     *
     *  @param  id              window id within application
     *  @param  width           desired width
     *  @param  height          desired height
     *  @param  name            desired name
     *  @param  pMonitor        pointer to a monitor where window
     *                          should be created
     *  @param  pSharedWindow   pointer to another window that
     *                          should share all resources with
     *                          created window
     */
    Window(uint32_t m_id,
        int32_t width,
        int32_t height,
        const std::string& name,
        Monitor* pMonitor = nullptr,
        Window* pSharedWindow = nullptr);

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;

    /** @brief  Destructs a window object */
    ~Window();

    /** @brief  Creates a vulkan surface that can be used with this window
     *
     *  @param[in]  instance    Vulkan instance
     *  @param[in]  allocator   Vulkan allocator
     *  @param[out] surface     variable that will hold created surface handle
     *
     *  @return Vulkan result flag
     */
    VkResult CreateVulkanSurface(VkInstance instance,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface);

    /** @brief  Checks if window should be closed */
    bool ShouldClose() const;

    /** @brief  Sets flag if window should be closed
     *
     *  Can be used to override user input when close event was emitted
     *  or just to set the flag if window should be closed
     */
    void SetShouldClose(bool flag) const;

    /** @brief  Returns the id of the window */
    uint32_t GetId() { return m_id; }

    /** @brief  Returns the name of the window */
    const std::string& GetName() const { return m_name; }

    /** @brief  Returns the size of the window */
    const std::pair<int32_t, int32_t>& GetSize() const { return m_size; }

    /** @brief  Returns the position of the window */
    const std::pair<int32_t, int32_t>& GetPosition() const { return m_position; }

    /** @brief  Returns a handle provided by window manager adapter */
    void* GetHandle() { return m_handle; }

    //! Signal that is emitted from destructor before the actual window is destroyed
    wink::signal< wink::slot<void(Window*)> > Destroyed;

    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > PositionChanged;
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > SizeChanged;
    wink::signal< wink::slot<void(Window*)> > Close;
    wink::signal< wink::slot<void(Window*)> > ContentRefresh;
    wink::signal< wink::slot<void(Window*, bool)> > Focused;
    wink::signal< wink::slot<void(Window*, bool)> > Minimized;
    wink::signal< wink::slot<void(Window*, bool)> > Maximized;
    wink::signal< wink::slot<void(Window*, std::pair<int32_t, int32_t>)> > FramebufferResized;

private:
    void OnWindowPositionChanged(void* handle, std::pair<int32_t, int32_t> position);
    void OnWindowSizeChanged(void* handle, std::pair<int32_t, int32_t> size);
    void OnWindowClose(void* handle);
    void OnWindowContentRefresh(void* handle);
    void OnWindowFocused(void* handle, bool flag);
    void OnWindowMinimized(void* handle, bool flag);
    void OnWindowMaximized(void* handle, bool flag);
    void OnWindowFramebufferResized(void* handle, std::pair<int32_t, int32_t> size);

    //! Window id within application
    uint32_t m_id;

    //! Window size in pixels as (width, height)
    std::pair<int32_t, int32_t> m_size;

    //! Window position in pixels as (width, height)
    std::pair<int32_t, int32_t> m_position;

    //! Window title name
    std::string m_name;

    //! Pointer to a monitor associated with the window
    Monitor* m_pMonitor;

    //! Pointer to another window that shares resources with this window
    Window* m_pSharedWindow;

    //! Handle provided by window manager adapter
    void* m_handle;
};

}
}

#endif // UNICORN_WINDOW_MANAGER_WINDOW_HPP
