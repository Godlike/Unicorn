/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_QUEUE_HPP
#define UNICORN_VIDEO_VULKAN_QUEUE_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Queue
{
public:
    Queue();
    ~Queue();

    void Init(vk::Device deivce);
    vk::Result Submit(vk::CommandBuffer cmdBuffer);
    vk::Result Submit(vk::CommandBuffer cmdBuffer, vk::Semaphore semaphore);
    vk::Result Wait();
    vk::Result Present(vk::SwapchainKHR swapchain, uint32_t index);

    vk::Queue& GetVkQueue();
private:
    vk::Device m_device;
    vk::Queue m_queue;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_QUEUE_HPP