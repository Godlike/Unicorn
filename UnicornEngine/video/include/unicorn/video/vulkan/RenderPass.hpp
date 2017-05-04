/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_RENDER_PASS_HPP
#define UNICORN_VIDEO_VULKAN_RENDER_PASS_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{

class RenderPass
{
public:
    RenderPass();
    ~RenderPass();

    vk::Result Create(vk::Device device, vk::Format colorFormat, vk::Format depthFormat);
    void Destroy();

    vk::RenderPass& GetVkRenderPass();
private:
    vk::Device m_device;
    vk::RenderPass m_renderPass;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_RENDER_PASS_HPP