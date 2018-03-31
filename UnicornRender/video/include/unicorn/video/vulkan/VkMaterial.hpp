/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VKMATERIAL_HPP
#define UNICORN_VIDEO_VULKAN_VKMATERIAL_HPP

#include <unicorn/video/vulkan/VkTexture.hpp>

#include <vulkan/vulkan.hpp>

#include <vector>

namespace unicorn
{
namespace video
{
namespace vulkan
{
/** @brief VkMaterial represents material in Vulkan renderer */
struct VkMaterial
{
    uint32_t handle = 0;
    std::vector<vk::DescriptorSet> descriptorSets;
    VkTexture* texture = nullptr;
    vk::Device device = nullptr;
    vk::DescriptorPool pool = nullptr;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_VKMATERIAL_HPP
