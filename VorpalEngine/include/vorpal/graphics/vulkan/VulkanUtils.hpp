/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_VULKANUTILS_HPP
#define VORPAL_GRAPHICS_VULKANUTILS_HPP

#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace vp
{
    void WaitVulkanDeviceIdle(const VkDevice& device);
    bool CheckValidationLayerSupport(const std::vector<const char*>& requiredLayers);    
}

#endif // VORPAL_GRAPHICS_VULKANUTILS_HPP
