/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_GRAPHICS_VULKANBUFFER_HPP
#define UNICORN_GRAPHICS_VULKANBUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace uc
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

#endif // UNICORN_GRAPHICS_VULKANBUFFER_HPP
