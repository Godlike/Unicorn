/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VkMesh.hpp>
#include <unicorn/video/Material.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
VkMesh::VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, Mesh& mesh)
    : m_valid(false)
    , m_device(device)
    , m_physicalDevice(physicalDevice)
    , m_pool(pool)
    , m_queue(queue)
    , m_mesh(mesh)
{
    m_mesh.MaterialUpdated.connect(this, &VkMesh::OnMaterialUpdated);
    m_mesh.VerticesUpdated.connect(this, &VkMesh::AllocateOnGPU);
}

VkMesh::~VkMesh()
{
    DeallocateOnGPU();
    m_mesh.VerticesUpdated.disconnect(this, &VkMesh::AllocateOnGPU);
    m_mesh.MaterialUpdated.disconnect(this, &VkMesh::OnMaterialUpdated);
}

bool VkMesh::operator==(const Mesh& mesh) const
{
    return &mesh == &m_mesh;
}

const glm::mat4& VkMesh::GetModelMatrix() const
{
    return m_mesh.GetModelMatrix();
}

Mesh const& VkMesh::GetMesh() const
{
    return m_mesh;
}

std::shared_ptr<Material> const VkMesh::GetMaterial() const
{
    return m_mesh.GetMaterial();
}

void VkMesh::AllocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
    Buffer stagingBuffer;
    //Vertexes filling
    auto size = sizeof(m_mesh.GetVertices()[0]) * m_mesh.GetVertices().size();
    stagingBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
    m_vertexBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, size);

    stagingBuffer.Map();
    stagingBuffer.Write(m_mesh.GetVertices().data());
    stagingBuffer.CopyToBuffer(m_pool, m_queue, m_vertexBuffer, m_vertexBuffer.GetSize());
    stagingBuffer.Destroy();
    //Indexes filling
    size = sizeof(m_mesh.GetIndices()[0]) * m_mesh.GetIndices().size();

    stagingBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
    stagingBuffer.Map();
    m_indexBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal, size);
    stagingBuffer.Write(m_mesh.GetIndices().data());
    stagingBuffer.CopyToBuffer(m_pool, m_queue, m_indexBuffer, m_indexBuffer.GetSize());
    stagingBuffer.Destroy();

    m_valid = true;
    ReallocatedOnGpu.emit(this);
}

void VkMesh::DeallocateOnGPU()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.Destroy();
}

vk::Buffer VkMesh::GetVertexBuffer() const
{
    return m_vertexBuffer.GetVkBuffer();
}

vk::Buffer VkMesh::GetIndexBuffer() const
{
    return m_indexBuffer.GetVkBuffer();
}
void VkMesh::OnMaterialUpdated()
{
    MaterialUpdated.emit(&m_mesh, this);
}
}
}
}
