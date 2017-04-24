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

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class Mesh : public geometry::Mesh
            {
            public:
                Mesh(vk::Device device, vk::PhysicalDevice physicalDevice);
                bool Create() override;
                vk::Buffer GetBuffer()
                {
                    return m_buffer.GetVkBuffer();
                }
            private:
                vk::Device m_device;
                vk::PhysicalDevice m_physicalDevice;
                vulkan::Buffer m_buffer;
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_MESH_HPP