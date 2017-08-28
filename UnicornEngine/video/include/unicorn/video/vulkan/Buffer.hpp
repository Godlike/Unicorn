/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_BUFFER_HPP
#define UNICORN_VIDEO_VULKAN_BUFFER_HPP

#include <unicorn/video/vulkan/Memory.hpp>

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Image;

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
     * @param [in] physicalDevice GPU for memory allocation
     * @param [in] device device for allocation
     * @param [in] usage buffer specific usage
     * @param [in] memoryPropertyFlags flags for memory usage
     * @param [in] size size of buffer
     * @return true if buffer was allocated correctly, false if some error occured
     */
    bool Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size);

    /**
     * @brief Destroys all buffer data
     */
    void Destroy();

    /**
     * @brief Writes data to buffer. You need to map it first.
     * @param [in] pData pointer to data content
     */
    void Write(void const* pData) const;

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
     * @param [in] pool pool for allocating commands from
     * @param [in] queue queue for processing
     * @param [in] dstBuffer destination buffer
     * @param [in] size size of copyable data
     */
    void CopyToBuffer(vk::CommandPool pool, vk::Queue queue, vulkan::Buffer& dstBuffer, vk::DeviceSize size) const;

    /**
     * @brief Copies buffer to image buffer
     * @param [in] dstImage destination image
     * @param [in] pool pool for allocating commands from
     * @param [in] queue queue for command buffers pushing
     */
    void CopyToImage(vulkan::Image const& dstImage, vk::CommandPool const& pool, vk::Queue const& queue) const;

    /**
     * @brief Getter for size of buffer
     * @return size of buffer
     */
    size_t GetSize() const;

    /**
     * @brief Getter for usage of this buffer
     * @return buffer usage flags
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
    const vk::Buffer& GetVkBuffer() const;

    const vk::DescriptorBufferInfo& GetDescriptorInfo() const;
private:
    vk::Device m_device;
    vk::BufferUsageFlags m_usage;
    size_t m_size;
    vk::Buffer m_buffer;
    Memory* m_deviceMemory;
    void* m_mappedMemory;
    vk::DescriptorBufferInfo m_descriptor;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_BUFFER_HPP
