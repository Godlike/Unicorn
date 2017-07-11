/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_MEMORY_HPP
#define UNICORN_VIDEO_VULKAN_MEMORY_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
/**
* @brief Wrapper for vk::DeviceMemory item in Vulkan API
*/
class Memory
{
public:
    Memory();
    ~Memory();
    vk::Result Allocate(vk::Device device, uint32_t typeFilter, vk::PhysicalDeviceMemoryProperties physMemProperties, vk::MemoryPropertyFlagBits reqMemProperties, uint64_t allocSize);
    void Free();
    vk::DeviceMemory& GetMemory();

    explicit operator bool() const
    {
        return m_memory;
    }
private:
    vk::Device m_device;
    vk::DeviceMemory m_memory;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MEMORY_HPP
