/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_IMAGE_HPP
#define UNICORN_VIDEO_VULKAN_IMAGE_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            class Image
            {
                vk::Device m_device;
                vk::DeviceMemory m_deviceMemory;
                vk::Image m_image;
                vk::ImageView m_imageView;

                vk::Format m_format;
                vk::ImageUsageFlags m_usage;
                int32_t m_width;
                int32_t m_height;
            public:
                Image();
                ~Image();

                Result create(
                    vk::Device device,
                    vk::Format format,
                    vk::ImageUsageFlags usage,
                    uint32_t width,
                    uint32_t height);
                void destroy();

                vk::Format getFormat();
                int32_t getWidth();
                int32_t getHeight();

                vk::Image& getVkImage();
                vk::ImageView& getVkImageView();
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_IMAGE_HPP