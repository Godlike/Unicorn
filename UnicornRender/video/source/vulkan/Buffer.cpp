/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Buffer.hpp>
#include <unicorn/video/vulkan/VulkanHelper.hpp>
#include <unicorn/video/vulkan/Image.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Buffer::Buffer() : m_size(0)
                 , m_deviceMemory(nullptr)
                 , m_mappedMemory(nullptr)
{
}

Buffer::~Buffer()
{
    Destroy();
}

bool Buffer::Create(vk::PhysicalDevice physicalDevice, vk::Device device,
                    vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryPropertyFlags, size_t size)
{
    if(size == 0)
    {
        LOG_VULKAN->Error("Can't allocate zero Vulkan buffer on gpu!");
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
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create vulkan buffer!");
        return false;
    }

    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties(&memoryProperties);

    vk::MemoryRequirements req;
    m_device.getBufferMemoryRequirements(m_buffer, &req);
    m_deviceMemory = new Memory(m_device, req.memoryTypeBits, memoryProperties,
                                vk::MemoryPropertyFlagBits::eHostVisible, req.size);
    if(!m_deviceMemory->IsInitialized())
    {
        LOG_VULKAN->Error("Can't allocate memory on gpu!");

        delete m_deviceMemory;
        m_deviceMemory = nullptr;

        return false;
    }
    m_descriptor.offset = 0;
    m_descriptor.buffer = m_buffer;
    m_descriptor.range = VK_WHOLE_SIZE;

    m_device.bindBufferMemory(m_buffer, m_deviceMemory->GetMemory(), 0);
    return true;
}

void Buffer::Destroy()
{
    Unmap();
    if(m_deviceMemory)
    {
        delete m_deviceMemory;
        m_deviceMemory = nullptr;
    }
    if(m_buffer)
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
        LOG_VULKAN->Warning("Can't write buffer, because it's not mapped!");
    }
}

void Buffer::Map()
{
    if(!m_mappedMemory)
    {
        m_device.mapMemory(m_deviceMemory->GetMemory(), 0, m_size, vk::MemoryMapFlagBits(), &m_mappedMemory);
    }
}

void Buffer::Unmap()
{
    if(m_mappedMemory)
    {
        m_device.unmapMemory(m_deviceMemory->GetMemory());
        m_mappedMemory = nullptr;
    }
}

void Buffer::CopyToBuffer(vk::CommandPool pool, vk::Queue queue, vulkan::Buffer& dstBuffer, vk::DeviceSize size) const
{
    vk::CommandBuffer commandBuffer = BeginSingleTimeCommands(m_device, pool);

    vk::BufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    commandBuffer.copyBuffer(m_buffer, dstBuffer.GetVkBuffer(), 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer, queue, m_device, pool);
}

void Buffer::CopyToImage(const vulkan::Image& dstImage, const vk::CommandPool& pool, const vk::Queue& queue) const
{
    vk::CommandBuffer commandBuffer = BeginSingleTimeCommands(m_device, pool);

    vk::BufferImageCopy region;
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = vk::Offset3D{0, 0, 0};
    region.imageExtent = vk::Extent3D{
        dstImage.GetWidth(),
        dstImage.GetHeight(),
        1
    };
    commandBuffer.copyBufferToImage(m_buffer, dstImage.GetVkImage(), vk::ImageLayout::eTransferDstOptimal, 1, &region);

    EndSingleTimeCommands(commandBuffer, queue, m_device, pool);
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
    return m_deviceMemory->GetMemory();
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
