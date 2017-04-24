/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Mesh.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            Mesh::Mesh(vk::Device device, vk::PhysicalDevice physicalDevice) : m_device(device), m_physicalDevice(physicalDevice)
            {
            }

            bool Mesh::Create()
            {
                auto size = sizeof(m_vertices[0]) * m_vertices.size();
                m_buffer.Create(m_physicalDevice, m_device, vk::BufferUsageFlagBits::eVertexBuffer, size);
                m_buffer.Write(m_vertices.data());
                return true;
            }
        }
    }
}