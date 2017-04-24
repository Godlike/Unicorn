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
#include <wink/slot.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class VkMesh
            {
            public:
                VkMesh(vk::Device device, vk::PhysicalDevice physicalDevice, std::shared_ptr<geometry::Mesh> mesh);
                void AllocateOnGPU();
                vk::Buffer GetBuffer();
                std::uint32_t VerticesSize();
            private:
                vk::Device m_device;
                vk::PhysicalDevice m_physicalDevice;
                vulkan::Buffer m_buffer;
                std::shared_ptr<geometry::Mesh> m_mesh;
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_MESH_HPP