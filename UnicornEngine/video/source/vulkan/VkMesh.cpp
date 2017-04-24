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
            VkMesh::VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, std::shared_ptr<geometry::Mesh> mesh) :
                m_device(device), m_physicalDevice(physicalDevice), m_mesh(mesh) 
            {
                // m_mesh->m_dataUpdated.connect(&VkMesh::AllocateOnGPU); TODO lel
            }

            void VkMesh::AllocateOnGPU()
            {
                m_buffer.Destroy(); // Do I need this?
                auto size = sizeof(m_mesh->m_vertices[0]) * m_mesh->m_vertices.size();
                m_buffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer, size);
                m_buffer.Write(m_mesh->m_vertices.data());
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