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
     * @brief Constructor for buffer
     */
    Buffer();
    /**
     * @brief Desctructor which calls Destroy
     */
    ~Buffer();

    /**
     * @brief Creates new buffer
     * @param physicalDevice GPU for memory allocation
     * @param device Device which will allocate memory on GPU
     * @param usage Buffer specific usage
     * @param memoryPropertyFlags Flags for memory usage
     * @param size Size of buffer
     * @return true if buffer was allocated correctly, false if some error occured
     */
    bool Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size);
    /**
     * @brief Destroys all buffer data
     */
    void Destroy();
    /**
     * @brief Writes data to buffer. You need to map it first.
     * @param pData Data which you want to fill buffer
     */
    void Write(const void* pData) const;
    /**
     * @brief Maps buffer
     */
    void Map();
    /**
     * @brief Unmaps buffer
     */
    void Unmap();
    /**
     * @brief Copies buffer to another buffer. Useful for staging buffering
     * @param pool Pool for allocating commands from
     * @param queue Queue for processing
     * @param dstBuffer Destination buffer
     * @param size Size of copyable data
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
    const vk::Buffer& GetVkBuffer();
    /**
    * @brief Getter for raw vk::Buffer
    * @return copy of vk::Buffer
    */
    vk::Buffer GetVkBuffer() const;

    const vk::DescriptorBufferInfo& GetDescriptorInfo() const;
private:
    vk::Device m_device;
    vk::BufferUsageFlags m_usage;
    size_t m_size;
    vk::Buffer m_buffer;
    vk::DeviceMemory m_memory;
    void* m_mappedMemory;
    vk::DescriptorBufferInfo m_descriptor;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_BUFFER_HPP
