/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Buffer.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Buffer::Buffer()
    : m_size(0), m_mappedMemory(nullptr)
{
}

Buffer::~Buffer()
{
    Destroy();
}

bool Buffer::Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size)
{
    if (size == 0)
    {
        LOG_ERROR("Can't allocate zero Vulkan buffer on gpu!");
        return false;
    }
    m_device = device;
    m_usage = usage;
    m_size = size;

    vk::BufferCreateInfo bufferInfo;
    bufferInfo.setUsage(m_usage);
    bufferInfo.setSize(m_size);
    bufferInfo.setQueueFamilyIndexCount(0);
    bufferInfo.setPQueueFamilyIndices(nullptr);
    bufferInfo.setSharingMode(vk::SharingMode::eExclusive);

    vk::Result result = m_device.createBuffer(&bufferInfo, nullptr, &m_buffer);
    if (result != vk::Result::eSuccess)
    {
        return false;
    }

    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties(&memoryProperties);

    vk::MemoryRequirements req;
    m_device.getBufferMemoryRequirements(m_buffer, &req);

    uint32_t memoryTypeBits = req.memoryTypeBits;
    uint32_t memoryTypeIndex = 0;
    for (unsigned int i = 0; i < (sizeof(memoryTypeBits) * 8); ++i)
    {
        if ((memoryTypeBits >> i) & 1)
        {
            if ( memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible )
            {
                memoryTypeIndex = i;
                break;
            }
        }
    }

    vk::MemoryAllocateInfo memoryInfo;
    memoryInfo.setMemoryTypeIndex(memoryTypeIndex);
    memoryInfo.setAllocationSize(req.size);

    result = m_device.allocateMemory(&memoryInfo, nullptr, &m_memory);
    if (result != vk::Result::eSuccess)
    {
        return false;
    }

    descriptor.offset = 0;
    descriptor.buffer = m_buffer;
    descriptor.range = VK_WHOLE_SIZE;

    #ifdef VKCPP_ENHANCED_MODE
    m_device.bindBufferMemory(m_buffer, m_memory, 0);
    #else
    m_device.bindBufferMemory(m_buffer, m_memory, 0);
    #endif
    return true;
}

void Buffer::Destroy()
{
    Unmap();
    if (m_memory)
    {
        m_device.freeMemory(m_memory);
        m_memory = nullptr;
    }
    if (m_buffer)
    {
        m_device.destroyBuffer(m_buffer);
        m_buffer = nullptr;
    }
}

void Buffer::Write(const void* pData) const
{
    memcpy(m_mappedMemory, pData, m_size);
}

void Buffer::Map()
{
    m_device.mapMemory(m_memory, 0, m_size, vk::MemoryMapFlagBits(), &m_mappedMemory);
}

void Buffer::Unmap()
{
    if (m_mappedMemory)
    {
        m_device.unmapMemory(m_memory);
        m_mappedMemory = nullptr;
    }
}

void Buffer::CopyToBuffer(vk::CommandPool pool, vk::Queue queue, const vulkan::Buffer& dstBuffer, vk::DeviceSize size)
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = pool;
    allocInfo.commandBufferCount = 1;

    vk::CommandBuffer commandBuffer;
    m_device.allocateCommandBuffers(&allocInfo, &commandBuffer);

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    commandBuffer.begin(&beginInfo);

    vk::BufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    commandBuffer.copyBuffer(m_buffer, dstBuffer.GetVkBuffer(), 1, &copyRegion);

    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    queue.submit(1, &submitInfo, nullptr); // TODO: parallelize this, move to another queue
    queue.waitIdle();
    m_device.freeCommandBuffers(pool, 1, &commandBuffer);
}

size_t Buffer::GetSize() const
{
    return m_size;
}

vk::BufferUsageFlags Buffer::GetUsage() const
{
    return m_usage;
}

vk::DeviceMemory Buffer::GetMemory() const
{
    return m_memory;
}

vk::Buffer& Buffer::GetVkBuffer()
{
    return m_buffer;
}

vk::Buffer Buffer::GetVkBuffer() const
{
    return m_buffer;
}
}
}
}
