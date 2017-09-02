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
 * @param[in] device device which allocate command buffers from
 * @param[in] commandPool pool which asignee command buffers to
 */
vk::CommandBuffer BeginSingleTimeCommands(const vk::Device& device,
                                          const vk::CommandPool& commandPool);

/*
* @brief Stops recording commands end executing queue
* @param[in] commandBuffer command buffer which needed to be execute
* @param[in] queue queue which execute to
* @param[in] device frees command buffers which was allocates from this device
* @param[in] commandPool holds command buffers to deallocation
*/
void EndSingleTimeCommands(const vk::CommandBuffer& commandBuffer,
                           const vk::Queue& queue,
                           const vk::Device& device,
                           const vk::CommandPool& commandPool);
}
}
}

#endif // UNICORN_VIDEO_VULKAN_VULKAN_HELPER_HPP
