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
            public:
                Image();
                ~Image();

                vk::Result Create(
                    vk::Device device,
                    vk::Format format,
                    vk::ImageUsageFlags usage,
                    uint32_t width,
                    uint32_t height);
                void Destroy();

                vk::Format GetFormat();
                int32_t GetWidth();
                int32_t GetHeight();

                vk::Image& GetVkImage();
                vk::ImageView& GetVkImageView();
            private:
                vk::Device m_device;
                vk::DeviceMemory m_deviceMemory;
                vk::Image m_image;
                vk::ImageView m_imageView;

                vk::Format m_format;
                vk::ImageUsageFlags m_usage;
                int32_t m_width;
                int32_t m_height;
            };
        }
    }
}

#endif // UNICORN_VIDEO_VULKAN_IMAGE_HPP