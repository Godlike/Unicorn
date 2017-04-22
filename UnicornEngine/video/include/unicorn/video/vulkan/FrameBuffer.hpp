/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_FRAMEBUFFER_HPP
#define UNICORN_VIDEO_VULKAN_FRAMEBUFFER_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class Framebuffer
            {
            public:
                Framebuffer();
                ~Framebuffer();

                Result create(
                    vk::Device device,
                    uint32_t width,
                    uint32_t height,
                    vk::ImageView colorImageView,
                    vk::ImageView depthImageView,
                    vk::RenderPass renderPass);
                void destroy();

                vk::Framebuffer& getVkFrameBuffer();
            private:
                vk::Device m_device;
                vk::Framebuffer m_framebuffer;
            };

        }
    }
}


#endif // UNICORN_VIDEO_VULKAN_FRAMEBUFFER_HPP