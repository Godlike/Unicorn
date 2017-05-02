/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VkMesh.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
VkMesh::VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, std::shared_ptr<geometry::Mesh> mesh)
    : m_device(device), m_physicalDevice(physicalDevice), m_mesh(mesh), m_pool(pool), m_queue(queue)
{
    mesh->DataUpdated.connect(this, &VkMesh::AllocateOnGPU);
}

void VkMesh::AllocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
    Buffer stagingBuffer;
    //Vertexes filling
    auto size = sizeof(m_mesh->GetVertices()[0]) * m_mesh->GetVertices().size();
    stagingBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
    m_vertexBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, size);

    stagingBuffer.Write(m_mesh->GetVertices().data());
    stagingBuffer.CopyToBuffer(m_pool, m_queue, m_vertexBuffer, m_vertexBuffer.GetSize());
    stagingBuffer.Destroy();
    //Indexes filling
    size = sizeof(m_mesh->GetIndices()[0]) * m_mesh->GetIndices().size();

    stagingBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
    m_indexBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, size);
    stagingBuffer.Write(m_mesh->GetIndices().data());
    stagingBuffer.CopyToBuffer(m_pool, m_queue, m_indexBuffer, m_indexBuffer.GetSize());
    stagingBuffer.Destroy();
    ReallocatedOnGpu.emit(this);
}

void VkMesh::DeallocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
}

vk::Buffer VkMesh::GetVertexBuffer()
{
    return m_vertexBuffer.GetVkBuffer();
}

std::uint32_t VkMesh::VerticesSize()
{
    return m_mesh->GetVertices().size();
}

vk::Buffer VkMesh::GetIndexBuffer()
{
    return m_indexBuffer.GetVkBuffer();
}

std::uint32_t VkMesh::IndicesSize()
{
    return m_mesh->GetIndices().size();
}
}
}
}