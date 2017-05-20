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
/**
 * @brief Wrapper for vk::Buffer item in Vulkan API
 */
class Buffer
{
public:
    /**
     * @brief Basic constructor for buffer
     */
    Buffer();
    /**
     * @brief Desctructor which calls @sa Destroy
     */
    ~Buffer();

    /**
     * @brief Create new buffer
     * @param physicalDevice vk::PhysicalDevice, which you want allocate on
     * @param device vk::Device which you want you for allocations
     * @param usage vk::BufferUsageFlags How you want to use this buffer
     * @param memoryPropertyFlags Flags for memory usage
     * @param size Size of buffer
     * @return true if buffer was allocated correctly, false if some error occured
     */
    bool Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size);
    /**
     * @brief Destroy all buffer data
     */
    void Destroy();
    /**
     * @brief Writing data to buffer. You need to map it first.
     * @param pData Data which you want to fill buffer
     */
    void Write(const void* pData) const;
    /**
     * @brief Mapping buffer
     */
    void Map();
    /**
     * @brief Unmapping buffer
     */
    void Unmap();
    /**
     * @brief Copying buffer to another buffer. Useful for staging buffering
     * @param pool Which pool you want to allocate commands from
     * @param queue Which queue you want to use 
     * @param dstBuffer Destination buffer
     * @param size Size of data, which you want to copy
     */
    void CopyToBuffer(vk::CommandPool pool, vk::Queue queue, const vulkan::Buffer& dstBuffer, vk::DeviceSize size);
    /**
     * @brief Getter for size of buffer
     * @return size of buffer
     */
    size_t GetSize() const;
    /**
     * @brief Getter for usage of this buffer
     * @return Buffer usage flags
     */
    vk::BufferUsageFlags GetUsage() const;
    /**
     * @brief Getter for memory where this buffer was allocated
     * @return device memory
     */
    vk::DeviceMemory GetMemory() const;
    /**
    * @brief Getter for raw vk::Buffer
    * @return reference to vk::Buffer
    */
    vk::Buffer& GetVkBuffer();
    /**
    * @brief Getter for raw vk::Buffer
    * @return copy of vk::Buffer
    */
    vk::Buffer  GetVkBuffer() const;

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
