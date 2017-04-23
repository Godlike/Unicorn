/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/FrameBuffer.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            Framebuffer::Framebuffer()
            {
            }

            Framebuffer::~Framebuffer()
            {
            }

            vk::Result Framebuffer::Create(
                vk::Device device,
                uint32_t width,
                uint32_t height,
                vk::ImageView colorImageView,
                vk::ImageView depthImageView,
                vk::RenderPass renderPass)
            {
                m_device = device;
                vk::ImageView attachments[2];
                attachments[0] = colorImageView;
                attachments[1] = depthImageView;

                vk::FramebufferCreateInfo framebufferInfo;
                framebufferInfo.setFlags(vk::FramebufferCreateFlagBits());
                framebufferInfo.setRenderPass(renderPass);
                framebufferInfo.setAttachmentCount(2);
                framebufferInfo.setPAttachments(attachments);
                framebufferInfo.setWidth(width);
                framebufferInfo.setHeight(height);
                framebufferInfo.setLayers(1);

                return  m_device.createFramebuffer(&framebufferInfo, nullptr, &m_framebuffer);
            }

            void Framebuffer::Destroy()
            {
                m_device.destroyFramebuffer(m_framebuffer, nullptr);
            }

            vk::Framebuffer& Framebuffer::GetVkFrameBuffer()
            {
                return m_framebuffer;
            }
        }
    }
}