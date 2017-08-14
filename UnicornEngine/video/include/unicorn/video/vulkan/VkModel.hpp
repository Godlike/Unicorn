/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_MODEL_HPP
#define UNICORN_VIDEO_VULKAN_MODEL_HPP

#include <wink/signal.hpp>
#include <unicorn/video/vulkan/VkMesh.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class VkModel
{
public:
    void AddVkMesh(VkMesh* mesh)
    {
        m_vkMeshes.push_back(mesh);
    }
    //! Array of renderer meshes
    std::list<VkMesh*> m_vkMeshes;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_MODEL_HPP
