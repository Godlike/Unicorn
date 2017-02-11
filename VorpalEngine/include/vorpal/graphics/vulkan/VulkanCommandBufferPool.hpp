/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/


#ifndef VORPAL_GRAPHICS_VULKAN_COMMAND_BUFFER_POOL_HPP
#define VORPAL_GRAPHICS_VULKAN_COMMAND_BUFFER_POOL_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vp
{
    namespace graphics
    {
        class VulkanCommandBufferPool
        {

            VkCommandPool m_commandPool;
        };
    }
}

#endif // VORPAL_GRAPHICS_VULKAN_COMMAND_BUFFER_POOL_HPP