/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_VULKANBUFFER_HPP
#define VORPAL_GRAPHICS_VULKANBUFFER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vp
{
namespace graphics
{
/**
* @brief Encapsulates access to a Vulkan buffer backed up by device memory.
* @note Must be filled by an external source.
*/
class VulkanBuffer
{
private:
    VkBuffer m_buffer;
    VkDevice m_device;
    VkDeviceMemory m_memory;
    VkDescriptorBufferInfo m_descriptor;
    VkDeviceSize m_size;
    VkDeviceSize m_alignment;
};
}
}

#endif // VORPAL_GRAPHICS_VULKANBUFFER_HPP
