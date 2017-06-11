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
/**
 * @brief Mesh info for Vulkan backend
 */
class VkMesh
{
public:
    /**
     * @brief Constructor
     * @param device Which device to use
     * @param physicalDevice Where to allocate mesh
     * @param pool Which pool to allocate commands from
     * @param queue Which queue to use to buffer copying
     * @param mesh Geometry data
     */
    VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, std::shared_ptr<geometry::Mesh> mesh);
    ~VkMesh();
    /**
     * @brief Allocation on GPU
     */
    void AllocateOnGPU();
    /**
     * @brief Deallocation on GPU
     */
    void DeallocateOnGPU();
    /**
     * @brief Returns vertex buffer
     * @return vk::Buffer
     */
    vk::Buffer GetVertexBuffer();
    /**
     * @brief Returns index buffer
     * @return vk::Buffer
     */
    vk::Buffer GetIndexBuffer();
    /**
     * @brief Matrix of transformations
     * @return Model matrix
     */
    glm::mat4 GetModel() const;
    /**
     * @brief Returns vertices size
     * @return size of vertices
     */
    std::uint32_t VerticesSize();
    /**
    * @briefReturns indices size
    * @return size of indices
    */
    std::uint32_t IndicesSize();
    /**
     * @brief Signal for command buffer reallocation
     */
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
