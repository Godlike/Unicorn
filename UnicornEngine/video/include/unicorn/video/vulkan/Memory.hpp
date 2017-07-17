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
    /**
     * @brief Creates object and allocates memory on device
     * @param device device for allocation
     * @param typeFilter bit field describing suitable memory types in @p physMemProperties
     * @param physMemProperties properties of memory on physical device
     * @param reqMemProperties required memory properties
     * @param allocSize size of allocation
     */
    Memory(vk::Device device, uint32_t typeFilter, vk::PhysicalDeviceMemoryProperties physMemProperties, vk::MemoryPropertyFlagBits reqMemProperties, uint64_t allocSize);
    /**
     * @brief Frees memory
     */
    ~Memory();

    Memory(const Memory& other) = delete;
    Memory(Memory&& other) = delete;
    Memory& operator=(Memory& other) = delete;
    Memory& operator=(Memory&& other) = delete;

    /**
    * @brief Checks if memory was initialized before
    * @return true if was initialized before and false if not
    */
    bool IsInitialized() const;

    /**
     * @brief Returns reference to vk::DeviceMemory
     * @return reference to vk::DeviceMemory
     */
    const vk::DeviceMemory& GetMemory() const;    
private:
    bool m_initialized;
    vk::Device m_device;
    vk::DeviceMemory m_memory;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MEMORY_HPP
