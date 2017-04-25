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
            VkMesh::VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, vk::CommandPool pool, vk::Queue queue, std::shared_ptr<geometry::Mesh> mesh) :
                m_device(device), m_physicalDevice(physicalDevice), m_mesh(mesh) , m_pool(pool), m_queue(queue)
            {
                mesh->DataUpdated.connect(this, &VkMesh::AllocateOnGPU);
            }

            void VkMesh::AllocateOnGPU()
            {
                m_buffer.Destroy();
                Buffer hostBuffer;
                auto size = sizeof(m_mesh->m_vertices[0]) * m_mesh->m_vertices.size();
                hostBuffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eTransferSrc, 
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, size);
                m_buffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, size);
                
                hostBuffer.Write(m_mesh->m_vertices.data());
                hostBuffer.CopyToBuffer(m_pool, m_queue, m_buffer, m_buffer.GetSize());
                hostBuffer.Destroy();
                ReallocatedOnGpu.emit(this);
            }

            vk::Buffer VkMesh::GetBuffer()
            {
                return m_buffer.GetVkBuffer();
            }

            std::uint32_t VkMesh::VerticesSize()
            {
                return m_mesh->VerticesSize();
            }
        }
    }
}