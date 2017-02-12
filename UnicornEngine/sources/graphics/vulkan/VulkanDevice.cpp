/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/graphics/Vulkan/VulkanDevice.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/utility/Logger.hpp>

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/,
    size_t /*location*/,
    int32_t /*code*/,
    const char* /*layerPrefix*/,
    const char* msg,
    void* /*userData*/)
{
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("VULKAN LAYER ERROR: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARNING("VULKAN LAYER WARNING: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER PERFORMANCE: %s", msg);
    }
    return VK_FALSE;
}

void uc::graphics::VulkanDevice::Init(const system::Window& renderWindow)
{
}

void uc::graphics::VulkanDevice::Deinit()
{

}

uc::graphics::VulkanDevice::~VulkanDevice()
{

}

uc::graphics::VulkanDevice::VulkanDevice()
{

}
