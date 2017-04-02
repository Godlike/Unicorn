/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP
#define UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP

#include <unicorn/window_manager/MonitorMemento.hpp>

#include <wink/signal.hpp>

#include <vulkan/vulkan.hpp>

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
    static void Init();
    static void Deinit();

    static void* CreateWindow(int32_t width,
        int32_t height,
        const char* name,
        void* pMonitorHandle,
        void* pSharedWindowHandle);

    static void DestroyWindow(void* pHandle);

    static bool IsVulkanSupported();

    static VkResult CreateVulkanSurface(VkInstance instance,
        void* pHandle,
        const VkAllocationCallbacks* allocator,
        VkSurfaceKHR* surface);

    static bool WindowShouldClose(void* pHandle);
    static void SetWindowShouldClose(void* pHandle, bool flag);

    static void GetWindowPosition(void* pHandle, int* x, int* y);
    static void SetWindowPosition(void* pHandle, int x, int y);

    static void PollEvents();

    static std::vector<const char*> GetRequiredVulkanExtensions();

    static std::vector<MonitorMemento> GetMonitors();

    static MonitorMemento GetMonitor(void* handle);

    static void SetGammaRamp(void* handle, const GammaRamp& gammaRamp);

    // Window events
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowPositionChanged;
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowSizeChanged;
    static wink::signal< wink::slot<void(void*)> > WindowClose;
    static wink::signal< wink::slot<void(void*)> > WindowContentRefresh;
    static wink::signal< wink::slot<void(void*, bool)> > WindowFocused;
    static wink::signal< wink::slot<void(void*, bool)> > WindowMinimized;
    static wink::signal< wink::slot<void(void*, bool)> > WindowMaximized;
    static wink::signal< wink::slot<void(void*, std::pair<int32_t, int32_t>)> > WindowFramebufferResized;

    // Monitor events
    static wink::signal< wink::slot<void(void*, MonitorMemento::State)> > MonitorStateChanged;

private:
    static bool s_isInitialized;
};

}
}
}

#endif /* UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP */
