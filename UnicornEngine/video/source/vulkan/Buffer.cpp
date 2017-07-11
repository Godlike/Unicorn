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
    : m_size(0), 
    m_mappedMemory(nullptr)
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
        LOG_ERROR("Can't create vulkan buffer!");
        return false;
    }

    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties(&memoryProperties);

    vk::MemoryRequirements req;
    m_device.getBufferMemoryRequirements(m_buffer, &req);

    result = m_deviceMemory.Allocate(m_device, req.memoryTypeBits, memoryProperties, vk::MemoryPropertyFlagBits::eHostVisible, req.size);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't allocate memory on gpu!");
        return false;
    }
    m_descriptor.offset = 0;
    m_descriptor.buffer = m_buffer;
    m_descriptor.range = VK_WHOLE_SIZE;

    m_device.bindBufferMemory(m_buffer, m_deviceMemory.GetMemory(), 0);
    return true;
}

void Buffer::Destroy()
{
    Unmap();
    if (m_deviceMemory)
    {
        m_deviceMemory.Free();
    }
    if (m_buffer)
    {
        m_device.destroyBuffer(m_buffer);
        m_buffer = nullptr;
    }
}

void Buffer::Write(const void* pData) const
{
    if(m_mappedMemory)
    {
        memcpy(m_mappedMemory, pData, m_size);
    }
    else
    {
        LOG_WARNING("Can't write buffer, because it's not mapped!");
    }
}

void Buffer::Map()
{
    if(!m_mappedMemory)
    {
        m_device.mapMemory(m_deviceMemory.GetMemory(), 0, m_size, vk::MemoryMapFlagBits(), &m_mappedMemory);
    }
}

void Buffer::Unmap()
{
    if (m_mappedMemory)
    {
        m_device.unmapMemory(m_deviceMemory.GetMemory());
        m_mappedMemory = nullptr;
    }
}

void Buffer::CopyToBuffer(vk::CommandPool pool, vk::Queue queue, vulkan::Buffer& dstBuffer, vk::DeviceSize size) const
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
    return m_deviceMemory.GetMemory();
}

const vk::Buffer& Buffer::GetVkBuffer() const
{
    return m_buffer;
}

const vk::DescriptorBufferInfo& Buffer::GetDescriptorInfo() const
{
    return m_descriptor;
}
}
}
}
