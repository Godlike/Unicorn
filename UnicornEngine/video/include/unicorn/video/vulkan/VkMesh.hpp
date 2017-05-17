/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_MESH_HPP
#define UNICORN_VIDEO_VULKAN_MESH_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/video/geometry/Mesh.hpp>
#include <unicorn/video/vulkan/Buffer.hpp>
#include <wink/signal.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class VkMesh
{
public:
    VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, std::shared_ptr<geometry::Mesh> mesh);
    ~VkMesh();
    void AllocateOnGPU();
    void DeallocateOnGPU();

    vk::Buffer GetVertexBuffer();
    vk::Buffer GetIndexBuffer();
    glm::mat4 GetModel() const;

    std::uint32_t VerticesSize();
    std::uint32_t IndicesSize();
    wink::signal<wink::slot<void(VkMesh*)>> ReallocatedOnGpu;
private:
    vk::Device m_device;
    vk::PhysicalDevice m_physicalDevice;
    vulkan::Buffer m_vertexBuffer, m_indexBuffer;
    std::shared_ptr<geometry::Mesh> m_mesh;
    vk::CommandPool m_pool;
    vk::Queue m_queue;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MESH_HPP