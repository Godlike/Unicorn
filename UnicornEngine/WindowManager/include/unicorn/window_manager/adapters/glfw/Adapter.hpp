/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP
#define UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <vector>

namespace unicorn
{
namespace WindowManager
{
namespace glfw
{

/** @brief  Proxies calls to glfw */
class Adapter
{
public:
    static void Init();
    static void Deinit();

    static void* CreateWindow(uint32_t width,
        uint32_t height,
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

    static void PollEvents();

    static std::vector<const char*> GetRequiredVulkanExtensions();

private:
    static bool s_isInitialized;
};

}
}
}

#endif /* UNICORN_WINDOW_MANAGER_ADAPTERS_GLFW_ADAPTER_HPP */
