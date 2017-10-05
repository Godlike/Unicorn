/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_MESH_HPP
#define UNICORN_VIDEO_VULKAN_MESH_HPP

#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/vulkan/Buffer.hpp>
#include <unicorn/video/vulkan/VkMaterial.hpp>

#include <vulkan/vulkan.hpp>
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
    VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, Mesh& mesh);
    ~VkMesh();

    /**
     *  @brief  Checks if VkMesh is operating on given mesh
     *
     *  @param  mesh    reference to geometry mesh
     *
     *  @return @c true if object operates on given mesh, @c false otherwise
     */
    bool operator==(const Mesh& mesh) const;

    /**
     *  @brief  Returns if VkMesh is valid to use
     *
     *  Generally VkMesh shall be used only if it is allocated on GPU
     */
    bool IsValid() const { return m_valid; }

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
     * @return vulkan buffer
     */
    vk::Buffer GetVertexBuffer() const;

    /**
     * @brief Returns index buffer
     * @return vulkan buffer
     */
    vk::Buffer GetIndexBuffer() const;

    /** @brief Returns model matrix */
    const glm::mat4& GetModelMatrix() const;

    /** @brief Returns vertices count */
    uint32_t VerticesSize() const;

    /** @brief Returns indices count */
    uint32_t IndicesSize() const;

    /** @brief Returns @c true if mesh is colored and @c false otherwise */
    bool IsColored() const;

    /** @brief Returns @c true if mesh is in wired mode and @c false otherwise */
    bool IsWired() const;
    bool isVisible() const;
    /**
     * @brief Returns color of mesh in RGB format
     * @return color of mesh in RGB format
     */
    glm::vec3 GetColor() const;

    /**
     * @brief Updates data if material of mesh was updated
     */
    void OnMaterialUpdated();

    /**
     * @brief Material in vulkan is combination of descriptor set and binded data   
     */
    std::shared_ptr<VkMaterial> pMaterial;

    /**
     * @brief Signal for command buffer reallocation
     */
    wink::signal<wink::slot<void(VkMesh*)>> ReallocatedOnGpu;

    /**
    * @brief Signal for material update
    */
    wink::signal<wink::slot<void(Mesh*, VkMesh*)>> MaterialUpdated;
private:
    bool m_valid;

    vk::Device m_device;
    vk::PhysicalDevice m_physicalDevice;
    vulkan::Buffer m_vertexBuffer, m_indexBuffer;
    vk::CommandPool m_pool;
    vk::Queue m_queue;

    Mesh& m_mesh;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MESH_HPP
