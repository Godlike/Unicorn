/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_VULKAN_HELPER_HPP
#define UNICORN_VIDEO_VULKAN_VULKAN_HELPER_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
vk::CommandBuffer BeginSingleTimeCommands(const vk::Device& device,
                                          const vk::CommandPool& commandPool);
void EndSingleTimeCommands(const vk::CommandBuffer& commandBuffer,
                           const vk::Queue& queue,
                           const vk::Device& device,
                           const vk::CommandPool& commandPool);
}
}
}

#endif // UNICORN_VIDEO_VULKAN_VULKAN_HELPER_HPP
