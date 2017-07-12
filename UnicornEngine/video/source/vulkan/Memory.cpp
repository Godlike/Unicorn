/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Memory.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Memory::Memory() : m_initialized(false)
                 , m_memory(nullptr)
{
}

Memory::~Memory()
{
    Free();
}

bool Memory::IsInitialized() const
{
    return m_initialized;
}

vk::Result Memory::Allocate(
    vk::Device device,
    uint32_t typeFilter,
    vk::PhysicalDeviceMemoryProperties physMemProperties,
    vk::MemoryPropertyFlagBits reqMemProperties,
    uint64_t allocSize)
{
    m_device = device;

    uint32_t memoryTypeIndex = 0;
    for (uint32_t i = 0; i < physMemProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i))
            && (physMemProperties.memoryTypes[i].propertyFlags & reqMemProperties) == reqMemProperties)
        {
            memoryTypeIndex = i;
        }
    }

    vk::MemoryAllocateInfo memoryInfo;
    memoryInfo.setMemoryTypeIndex(memoryTypeIndex);
    memoryInfo.setAllocationSize(allocSize);

    vk::Result result = m_device.allocateMemory(&memoryInfo, nullptr, &m_memory);
    if(result == vk::Result::eSuccess)
    {
        m_initialized = true;
    }
    return result;
}

void Memory::Free()
{
    if(m_memory)
    {
        m_device.freeMemory(m_memory);
        m_memory = nullptr;
    }
    m_initialized = false;
}

const vk::DeviceMemory& Memory::GetMemory() const
{
    return m_memory;
}

Memory::operator bool() const
{
    return m_memory;
}
}
}
}
