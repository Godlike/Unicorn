/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/window_manager/adapters/glfw/Adapter.hpp>

#include <GLFW/glfw3.h>

namespace unicorn
{
namespace WindowManager
{
namespace glfw
{

bool Adapter::s_isInitialized = false;

void Adapter::Init()
{
    if (!s_isInitialized)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        s_isInitialized = true;
    }
}

void Adapter::Deinit()
{
    if (s_isInitialized)
    {
        glfwTerminate();

        s_isInitialized = false;
    }
}

void* Adapter::CreateWindow(uint32_t width,
    uint32_t height,
    const char* name,
    void* pMonitorHandle,
    void* pSharedWindowHandle)
{
    return glfwCreateWindow(width
        , height
        , name
        , static_cast<GLFWmonitor*>(pMonitorHandle)
        , static_cast<GLFWwindow*>(pSharedWindowHandle)
    );
}

void Adapter::DestroyWindow(void* pHandle)
{
    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(pHandle), GLFW_TRUE);
    glfwDestroyWindow(static_cast<GLFWwindow*>(pHandle));
}

bool Adapter::IsVulkanSupported()
{
    return glfwVulkanSupported();
}

VkResult Adapter::CreateVulkanSurface(VkInstance instance,
    void* pHandle,
    const VkAllocationCallbacks* allocator,
    VkSurfaceKHR* surface)
{
    return glfwCreateWindowSurface(instance
        , static_cast<GLFWwindow*>(pHandle)
        , allocator
        , surface
    );
}

bool Adapter::WindowShouldClose(void* pHandle)
{
    return glfwWindowShouldClose(static_cast<GLFWwindow*>(pHandle));
}

void Adapter::PollEvents()
{
    glfwPollEvents();
}

std::vector<const char*> Adapter::GetRequiredVulkanExtensions()
{
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; ++i)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    return extensions;
}

}
}
}
