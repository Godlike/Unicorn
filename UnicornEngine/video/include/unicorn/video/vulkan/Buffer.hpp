/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_BUFFER_HPP
#define UNICORN_VIDEO_VULKAN_BUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Buffer
{
public:
    Buffer();
    ~Buffer();

    bool Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size);
    void Destroy();
    void Write(const void* pData) const;
    void MappedWrite(const void* pData) const;
    void Map();
    void Unmap();
    void CopyToBuffer(vk::CommandPool pool, vk::Queue queue, const vulkan::Buffer& dstBuffer, vk::DeviceSize size);
    vk::Buffer GetVkBuffer() const;
    size_t GetSize() const;
    vk::BufferUsageFlags GetUsage() const;
    vk::DeviceMemory GetMemory() const;

    vk::Buffer& GetVkBuffer();

    vk::DescriptorBufferInfo descriptor;
private:
    vk::Device m_device;
    vk::BufferUsageFlags m_usage;
    size_t m_size;
    vk::Buffer m_buffer;
    vk::DeviceMemory m_memory;
    void* m_mappedMemory;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_BUFFER_HPP