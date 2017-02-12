/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_VULKANUTILS_HPP
#define UNICORN_GRAPHICS_VULKANUTILS_HPP

#include <vector>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace uc
{
    void WaitVulkanDeviceIdle(const VkDevice& device);
    bool CheckValidationLayerSupport(const std::vector<const char*>& requiredLayers);    
}

#endif // UNICORN_GRAPHICS_VULKANUTILS_HPP
