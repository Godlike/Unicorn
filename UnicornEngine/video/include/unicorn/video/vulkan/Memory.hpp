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
    /**
     * @brief Allocates memory on device
     * @param device device for allocation
     * @param typeFilter bit field of memory types that are suitable
     * @param physMemProperties properties of memory on physical device 
     * @param reqMemProperties required memory properties
     * @param allocSize size of allocation
     * @return result of allocation
     */
    vk::Result Allocate(vk::Device device, uint32_t typeFilter, vk::PhysicalDeviceMemoryProperties physMemProperties, vk::MemoryPropertyFlagBits reqMemProperties, uint64_t allocSize);
    /**
     * @brief Frees memory
     */
    void Free();
    /**
     * @brief Returns reference to vk::DeviceMemory
     * @return reference to vk::DeviceMemory
     */
    const vk::DeviceMemory& GetMemory() const;

    explicit operator bool() const;
private:
    vk::Device m_device;
    vk::DeviceMemory m_memory;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MEMORY_HPP
