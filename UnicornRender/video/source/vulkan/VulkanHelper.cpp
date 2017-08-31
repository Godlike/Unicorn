/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VulkanHelper.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
vk::CommandBuffer BeginSingleTimeCommands(const vk::Device& device,
                                          const vk::CommandPool& commandPool)
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    vk::CommandBuffer commandBuffer;
    device.allocateCommandBuffers(&allocInfo, &commandBuffer);

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    commandBuffer.begin(&beginInfo);

    return commandBuffer;
}

void EndSingleTimeCommands(const vk::CommandBuffer& commandBuffer,
                           const vk::Queue& queue,
                           const vk::Device& device,
                           const vk::CommandPool& commandPool)
{
    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    queue.submit(1, &submitInfo, nullptr);
    queue.waitIdle();
    device.freeCommandBuffers(commandPool, 1, &commandBuffer);
}
}
}
}
