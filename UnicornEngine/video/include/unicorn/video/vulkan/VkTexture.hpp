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
/**
 * @brief VkTexture represents texture at Vulkan backend
 */
class VkTexture
{
public:
    VkTexture(vk::Device device);

    /**
     * @brief 
     * @return constant reference to descriptor image info
     */
    const vk::DescriptorImageInfo& GetDescriptorImageInfo() const;

    /**
     * @brief 
     * @param physicalDevice 
     * @param device 
     * @param commandPool 
     * @param queue 
     * @param texture 
     * @return 
     */
    bool Create(vk::PhysicalDevice const& physicalDevice, vk::Device const& device,
                vk::CommandPool const& commandPool, vk::Queue const& queue, Texture const* texture);

    /**
     * @brief 
     */
    void Delete();
    /**
     * @brief 
     * @return 
     */
    bool IsInitialized() const;
private:
    vk::Device m_device;
    vk::DescriptorImageInfo m_imageInfo;
    Image* m_vkImage;
    vk::Sampler m_sampler;
    bool m_isInitialized;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_TEXTURE_HPP
