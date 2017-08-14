/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_TEXTURE_HPP
#define UNICORN_VIDEO_VULKAN_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/video/vulkan/Image.hpp>
#include <wink/signal.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class VkTexture
{
public:
    VkTexture(unsigned char* data, int32_t width, int32_t height)
    {

        m_imageInfo.imageView = textureImageView;
        m_imageInfo.sampler = textureSampler;
    }
    const vk::DescriptorImageInfo& GetDescriptorImageInfo()
    {
        return m_imageInfo;
    }

private:
    vk::DescriptorImageInfo m_imageInfo;
    Image m_VkImage;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_TEXTURE_HPP
