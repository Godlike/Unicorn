/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/CommandBuffers.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
CommandBuffers::CommandBuffers()
{
}

CommandBuffers::~CommandBuffers()
{
}

vk::Result CommandBuffers::Create(vk::Device device, size_t count)
{
    m_device = device;
    vk::CommandPoolCreateInfo cmdPoolInfo;
    cmdPoolInfo.setQueueFamilyIndex(0);
    cmdPoolInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    vk::Result result = m_device.createCommandPool(&cmdPoolInfo, nullptr, &m_cmdPool);
    if ( result != vk::Result::eSuccess )
    {
        return result;
    }

    vk::CommandBufferAllocateInfo cmdBufAllocateInfo;
    cmdBufAllocateInfo.setCommandPool(m_cmdPool);
    cmdBufAllocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
    cmdBufAllocateInfo.setCommandBufferCount(count);

    m_cmdBuffers.resize(count);
    result = m_device.allocateCommandBuffers(&cmdBufAllocateInfo, m_cmdBuffers.data());
    return result;
}

void CommandBuffers::Destroy() const
{
    m_device.destroyCommandPool(m_cmdPool, nullptr);;
}

vk::CommandPool& CommandBuffers::GetVkCommandPool()
{
    return m_cmdPool;
}
vk::CommandBuffer& CommandBuffers::GetVkCommandBuffer(size_t index)
{
    return m_cmdBuffers.at(index);
}
const std::vector<vk::CommandBuffer>& CommandBuffers::GetVkCommandBuffers()
{
    return m_cmdBuffers;
}
}
}
}