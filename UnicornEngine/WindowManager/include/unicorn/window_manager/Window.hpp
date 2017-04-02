/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_WINDOW_HPP
#define UNICORN_WINDOW_MANAGER_WINDOW_HPP

#include <unicorn/window_manager/Monitor.hpp>

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <string>

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
    Window(uint32_t m_id
        , uint32_t width
        , uint32_t height
        , const std::string& name
        , Monitor* pMonitor = nullptr
        , Window* pSharedWindow = nullptr);

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

    /** @brief  Returns the id of the window */
    uint32_t GetId() { return m_id; }

    /** @brief  Returns a handle provided by window manager adapter */
    void* GetHandle() { return m_handle; }

private:
    //! Window id within application
    uint32_t m_id;

    //! Width in pixels
    uint32_t m_width;

    //! Height in pixels
    uint32_t m_height;

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
