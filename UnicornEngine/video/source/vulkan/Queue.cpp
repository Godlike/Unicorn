/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Queue.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Queue::Queue()
{
}
Queue::~Queue()
{
}

void Queue::Init(vk::Device device)
{
    m_device = device;
    m_device.getQueue(0, 0, &m_queue);
}

vk::Result Queue::Submit(vk::CommandBuffer cmdBuffer)
{
    vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eBottomOfPipe;

    vk::SubmitInfo submitInfo;
    submitInfo.setWaitSemaphoreCount(0);
    submitInfo.setPWaitSemaphores(nullptr);
    submitInfo.setSignalSemaphoreCount(0);
    submitInfo.setPSignalSemaphores(nullptr);
    submitInfo.setPWaitDstStageMask(&stage);
    submitInfo.setCommandBufferCount(1);
    submitInfo.setPCommandBuffers(&cmdBuffer);

    vk::Result result = m_queue.submit(1, &submitInfo, vk::Fence());
    return result;
}

vk::Result Queue::Submit(vk::CommandBuffer cmdBuffer, vk::Semaphore semaphore)
{
    vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eBottomOfPipe;

    vk::SubmitInfo submitInfo;
    submitInfo.setWaitSemaphoreCount(1);
    submitInfo.setPWaitSemaphores(&semaphore);
    submitInfo.setSignalSemaphoreCount(0);
    submitInfo.setPSignalSemaphores(nullptr);
    submitInfo.setPWaitDstStageMask(&stage);
    submitInfo.setCommandBufferCount(1);
    submitInfo.setPCommandBuffers(&cmdBuffer);

    vk::Result result = m_queue.submit(1, &submitInfo, vk::Fence());
    return result;
}

vk::Result Queue::Wait()
{
    #ifdef VKCPP_ENHANCED_MODE
    m_queue.waitIdle();
    return Result::eSuccess;
    #else
    vk::Result result = m_queue.waitIdle();
    return result;
    #endif
}

vk::Result Queue::Present(vk::SwapchainKHR swapchain, uint32_t index)
{
    vk::PresentInfoKHR presentInfo;
    presentInfo.setSwapchainCount(1);
    presentInfo.setPSwapchains(&swapchain);
    presentInfo.setPImageIndices(&index);
    presentInfo.setWaitSemaphoreCount(0);
    presentInfo.setPWaitSemaphores(nullptr);
    presentInfo.setPResults(nullptr);

    vk::Result result = m_queue.presentKHR(&presentInfo);
    return result;
}

vk::Queue& Queue::GetVkQueue()
{
    return m_queue;
}
}
}
}