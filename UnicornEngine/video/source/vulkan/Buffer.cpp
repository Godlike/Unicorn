#include <unicorn/video/vulkan/Buffer.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

vk::Result Buffer::Create(vk::PhysicalDevice physicalDevice, vk::Device device, vk::BufferUsageFlags usage, size_t size)
{
    m_device = device;
    m_usage = usage;
    m_size = size;

    vk::Result result;

    //init BufferCreateInfo
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.setFlags(vk::BufferCreateFlagBits());
    bufferInfo.setUsage(m_usage);
    bufferInfo.setSize(m_size);
    bufferInfo.setQueueFamilyIndexCount(0);
    bufferInfo.setPQueueFamilyIndices(nullptr);
    bufferInfo.setSharingMode(vk::SharingMode::eExclusive);

    //create VertexBuffer
    result = m_device.createBuffer(&bufferInfo, nullptr, &m_buffer);
    if (result != vk::Result::eSuccess)
    {
        return result;
    }

    //get PhysicalDeviceMemoryProperties
    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties(&memoryProperties);

    //get MemoryRequirements
    vk::MemoryRequirements req;
    m_device.getBufferMemoryRequirements(m_buffer, &req);

    uint32_t memoryTypeBits = req.memoryTypeBits;
    uint32_t memoryTypeIndex = 0;
    for (int i = 0; i < (sizeof(memoryTypeBits) * 8); ++i)
    {
        if ((memoryTypeBits >> i) & 1)
        {
            if (memoryProperties.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible)
            {
                memoryTypeIndex = i;
                break;
            }
        }
    }

    //init MemoryAllocateInfo
    vk::MemoryAllocateInfo memoryInfo;
    memoryInfo.setMemoryTypeIndex(memoryTypeIndex);
    memoryInfo.setAllocationSize(req.size);

    //allocate VertexMemory
    result = m_device.allocateMemory(&memoryInfo, nullptr, &m_memory);
    if (result != vk::Result::eSuccess)
    {
        return result;
    }

//bind VertexMemory to VertexBuffer
#ifdef VKCPP_ENHANCED_MODE
    m_device.bindBufferMemory(m_buffer, m_memory, 0);
#else
    result = m_device.bindBufferMemory(m_buffer, m_memory, 0);
#endif
    return result;
}

void Buffer::Destroy() const
{
    m_device.freeMemory(m_memory, nullptr);
    m_device.destroyBuffer(m_buffer, nullptr);
}

void Buffer::Write(const void* pData) const
{
    void* mappedMemory = nullptr;

#ifdef VKCPP_ENHANCED_MODE
    mappedMemory = m_device.mapMemory(m_memory, 0, m_size, vk::MemoryMapFlagBits());
#else
    vk::Result result = m_device.mapMemory(m_memory, 0, m_size, vk::MemoryMapFlagBits(), &mappedMemory);
#endif
    //copy Vertexes
    memcpy(mappedMemory, pData, m_size);

    //unmap VertexMemory
    m_device.unmapMemory(m_memory);
}

size_t Buffer::GetSize() const
{
    return m_size;
}

vk::Buffer& Buffer::GetVkBuffer()
{
    return m_buffer;
}
}
}
}