/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP
#define UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP

#include <unicorn/window_manager/MonitorMemento.hpp>
#include <unicorn/window_manager/WindowAttribute.hpp>
#include <unicorn/window_manager/WindowHint.hpp>

#include <wink/signal.hpp>

#include <vulkan/vulkan.hpp>

#include <cmath>
#include <cstdint>
#include <utility>
#include <vector>

namespace unicorn
{
namespace WindowManager
{

struct GammaRamp;

namespace glfw
{

/** @brief  Proxies calls to glfw */
class Adapter
{
public:
    /** @brief  Initializes glfw subsystem */
    static void Init();

    /** @brief  Deinitializes glfw subsystem */
    static void Deinit();

    /** @brief  Sets window creation hints
     *
     *  @param  hint    window creation hint
     *  @param  value   new value
     */
    static void SetWindowCreationHint(WindowManager::WindowHint hint, int32_t value);

    /** @brief  Resets window creation hints */
    static void ResetWindowHints();

    /** @brief  Creates a window with given properties
     *
     *  @param  width               desired width
     *  @param  height              desired height
     *  @param  name                window name
     *  @param  monitorHandle       monitor handle if fullscreen, @c nullptr
     *                              if windowed
     *  @param  sharedWindowHandle  pointer to a window handle to share
     *                              context and resources with
     *
     *  @return handle to newly created window
     */
    static void* CreateWindow(int32_t width,
        int32_t height,
        const char* name,
        void* monitorHandle,
        void* sharedWindowHandle);

    /** @brief  Destroys a window
     *
     *  @param  handle  window to be destroyed
     */
    static void DestroyWindow(void* handle);

    /** @brief  Returns @c true if vulkan can be used */
    static bool IsVulkanSupported();

    /** @brief  Creates a vulkan surface
     *
     *  @param[in]  instance    vulkan instance
     *  @param[in]  handle      window handle
     *  @param[in]  allocator   vulkan allocator
     *  @param[out] surface     stores created vulkan surface
     *
     *  @return vulkan result flag
     */
    static VkResult CreateVulkanSurface(VkInstance instance,
        void* handle,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface);

    /** @brief  Returns @c true if given window should be closed
     *
     *  @param  handle  window handle
     *
     *  @return @c true if given window should be closed, @c false otherwise
     */
    static bool WindowShouldClose(void* handle);

    /** @brief  Sets the flag indicating if given window should be closed
     *
     *  Can be used to override user input when close event was emitted
     *  or just to set the flag if window should be closed
     *
     *  @param  handle  window handle
     *  @param  flag    new value
     */
    static void SetWindowShouldClose(void* handle, bool flag);

    /** @brief  Gets window position
     *
     *  @param[in]  handle  window handle
     *  @param[out] x       stores x coordinate
     *  @param[out] y       stores y coordinate
     */
    static void GetWindowPosition(void* handle, int* x, int* y);

    /** @brief  Sets window position
     *
     *  @param  handle  window handle
     *  @param  x       x coordinate
     *  @param  y       y coordinate
     */
    static void SetWindowPosition(void* handle, int x, int y);

    /** @brief  Gets window size
     *
     *  @param[in]  handle  window handle
     *  @param[out] width   stores width
     *  @param[out] height  stores height
     */
    static void GetWindowSize(void* handle, int* width, int* height);

    /** @brief  Sets window size
     *
     *  @param  handle  window handle
     *  @param  width   width
     *  @param  height  height
     */
    static void SetWindowSize(void* handle, int width, int height);

    /** @brief  Sets window size limits
     *
     *  @param  handle  window handle
     *  @param  minSize pair of lower boundaries as (width, height)
     *  @param  maxSize pair of higher boundaries as (width, height)
     */
    static void SetWindowSizeLimits(void* handle, std::pair<int32_t, int32_t> minSize, std::pair<int32_t, int32_t> maxSize);

    /** @brief  Sets window aspect ration
     *
     *  @param  handle  window handle
     *  @param  ratio   pair of values as (width, height)
     */
    static void SetWindowAspectRatio(void* handle, std::pair<int32_t, int32_t> ratio);

    /** @brief  Minimizes window
     *
     *  @param  handle  window handle
     */
    static void MinimizeWindow(void* handle);

    /** @brief  Restores window from minimize/maximize
     *
     *  @param  handle  window handle
     */
    static void RestoreWindow(void* handle);

    /** @brief  Maximizes window
     *
     *  @param  handle  window handle
     */
    static void MaximizeWindow(void* handle);

    /** @brief  Brings window to front and sets input focus
     *
     *  @param  handle  window handle
     */
    static void FocusWindow(void* handle);

    /** @brief  Makes window invisible
     *
     *  @param  handle  window handle
     */
    static void HideWindow(void* handle);

    /** @brief  Makes window visible
     *
     *  @param  handle  window handle
     */
    static void ShowWindow(void* handle);

    /** @brief  Sets the name of the window
     *
     *  @param  handle  window handle
     *  @param  name    new name of the window
     */
    static void SetWindowName(void* handle, const char* name);

    /** @brief  Returns monitor associated with given window
     *
     *  @param  handle  window handle
     *
     *  @return a handle to monitor associated with given window or @c nullptr
     *          if something went wrong or window is not in fullscreen mode
     */
    static void* GetWindowMonitor(void* handle);

    /** @brief  Sets monitor to be used by window for fullscreen mode
     *
     *  @param  windowHandle    window handle
     *  @param  monitorHandle   monitor handle or @c nullptr for windowed mode
     *  @param  position        desired position as (x, y)
     *  @param  size            desired size as (width, height)
     *  @param  refreshRate     desired refresh rate
     */
    static void SetWindowMonitor(void* windowHandle,
        void* monitorHandle,
        std::pair<int32_t, int32_t> position,
        std::pair<int32_t, int32_t> size,
        int32_t refreshRate);

    /** @brief  Returns a value for window attribute
     *
     *  @param  handle      window handle
     *  @param  attribute   attribute
     *
     *  @return attribute value
     */
    static int32_t GetWindowAttribute(void* handle, WindowManager::WindowAttribute attribute);

    /** @brief  Processes events that are in event queue */
    static void PollEvents();

    /** @brief  Waits for events to be processed and processes them
     *
     *  Puts thread into sleep until at least one event is available in the
     *  event queue or the specified timeout @p timeoutSeconds is reached.
     *
     *  @param  timeoutSeconds  sleep limit
     */
    static void WaitEvents(double timeoutSeconds = NAN);

    /** @brief  Returns a vector of vulkan extensions required by glfw */
    static std::vector<const char*> GetRequiredVulkanExtensions();

    /** @brief  Returns a vector of monitor descriptions */
    static std::vector<MonitorMemento> GetMonitors();

    /** @brief  Returns a monitor description
     *
     *  @param  handle  monitor handle
     *
     *  @return monitor description
     */
    static MonitorMemento GetMonitor(void* handle);

    /** @brief  Sets gamma ramp for monitor
     *
     *  @param  handle      monitor handle
     *  @param  gammaRamp   gammaRamp description
     */
    static void SetGammaRamp(void* handle, const GammaRamp& gammaRamp);

    /** @name Window events */
    //! @{

    /** @brief  Event triggered when window position is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (x, y)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowPositionChanged;

    /** @brief  Event triggered when window size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (width, height)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowSizeChanged;

    /** @brief  Event triggered when window is going to be closed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     */
    static wink::signal< wink::slot<void(void*)> > WindowClose;

    /** @brief  Event triggered when window content should be refreshed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     */
    static wink::signal< wink::slot<void(void*)> > WindowContentRefresh;

    /** @brief  Event triggered when window focus is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowFocused;

    /** @brief  Event triggered when window minimized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowMinimized;

    /** @brief  Event triggered when window maximized state is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# boolean flag
     */
    static wink::signal< wink::slot<void(void*, bool)> > WindowMaximized;

    /** @brief  Event triggered when window framebuffer size is changed
     *
     *  Event is emitted with the following signature:
     *  -# window handle
     *  -# pair of values as (width, height)
     */
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowFramebufferResized;

    //! @}

    /** @name Monitor events */
    //! @{

    /** @brief  Event triggered when monitor state is changed (connected/disconnected)
     *
     *  Event is emitted with the following signature:
     *  -# monitor handle
     *  -# monitor state
     */
    static wink::signal< wink::slot<void(void*, MonitorMemento::State)> > MonitorStateChanged;

    //! @}

private:
    /** @brief  Converts WindowManager hint to glfw hint */
    static int ConvertToGlfwHint(WindowManager::WindowHint hint);

    /** @brief  Converts WindowManager attribute to glfw attribute */
    static int ConvertToGlfwAttribute(WindowManager::WindowAttribute attribute);

    /** @brief  Tries to convert WindowManager value to glfw value */
    static int ConvertToGlfwValue(int32_t value);

    //! Flag indicating if glfw was initialized
    static bool s_isInitialized;
};

}
}
}

#endif /* UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP */
