/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_TEXTURE_HPP
#define UNICORN_VIDEO_VULKAN_TEXTURE_HPP

#include <unicorn/video/vulkan/Image.hpp>
#include <unicorn/utility/Logger.hpp>

#include <vulkan/vulkan.hpp>
#include <wink/signal.hpp>



namespace unicorn
{
namespace video
{
class Texture;
namespace vulkan
{
class VkTexture
{
public:
        VkTexture();
    
        const vk::DescriptorImageInfo& GetDescriptorImageInfo();
        bool Create(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::CommandPool& commandPool, const vk::Queue& queue, const Texture* texture);
private:
    vk::DescriptorImageInfo m_imageInfo;
    Image* m_vkImage;
    vk::Sampler m_sampler;
    bool m_isInitialized;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_TEXTURE_HPP
