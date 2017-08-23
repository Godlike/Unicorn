/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_MESH_HPP
#define UNICORN_VIDEO_VULKAN_MESH_HPP

#include <unicorn/video/Mesh.hpp>
#include <unicorn/video/vulkan/Buffer.hpp>
#include <unicorn/video/vulkan/VkTexture.hpp>

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
    /**
     * @brief Matrix of transformations
     * @return model matrix
     */
    const glm::mat4& GetModel() const;
    /**
     * @brief Returns vertices size
     * @return size of vertices
     */
    uint32_t VerticesSize() const;
    /**
    * @briefReturns indices size
    * @return size of indices
    */
    uint32_t IndicesSize() const;

    bool IsColored() const
    {
        return m_isColored;
    }

    bool IsWired() const
    {
        return m_isWired;
    }

    glm::vec3 GetColor() const
    {
        return m_color;
    }

    void SetTextureHandler(uint32_t handler)
    {
        m_textureHandler = handler;
    }

    uint32_t GetTextureHandler() const
    {
        return m_textureHandler;
    }

    vk::DescriptorSet& GetDesctiptionSet()
    {
        return m_descriptorSet;
    }

    /**
     * @brief Signal for command buffer reallocation
     */
    wink::signal<wink::slot<void(VkMesh*)>> ReallocatedOnGpu;
private:
    bool m_valid;
    bool m_isColored;
    bool m_isWired;

    vk::Device m_device;
    vk::PhysicalDevice m_physicalDevice;
    vulkan::Buffer m_vertexBuffer, m_indexBuffer;
    vk::CommandPool m_pool;
    vk::Queue m_queue;

    Mesh& m_mesh;
    glm::vec3 m_color;
    uint32_t m_textureHandler;

    vk::DescriptorSet m_descriptorSet;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MESH_HPP
