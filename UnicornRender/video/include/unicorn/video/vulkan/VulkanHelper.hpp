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
/*
 * @brief Begin recording commands
 * @param[in] @param[in] device used to create command buffers
 * @param[out] commandPool pool where command buffers are allocated
 */
vk::CommandBuffer BeginSingleTimeCommands(const vk::Device& device,
                                          const vk::CommandPool& commandPool);

/*
* @brief Stops recording commands end executing queue
* @param[in] commandBuffer command buffer which needs to be executed
* @param[out] queue used to store commands for execution
* @param[in] @param[in] device used when creating @p commandBuffer
* @param[out] commandPool holds command buffers
*/
void EndSingleTimeCommands(const vk::CommandBuffer& commandBuffer,
                           const vk::Queue& queue,
                           const vk::Device& device,
                           const vk::CommandPool& commandPool);
}
}
}

#endif // UNICORN_VIDEO_VULKAN_VULKAN_HELPER_HPP
