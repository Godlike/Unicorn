/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_TEXTURE_HPP
#define UNICORN_VIDEO_VULKAN_TEXTURE_HPP

#include <unicorn/video/vulkan/Image.hpp>
#include <mule/Logger.hpp>

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
    /**
     * @brief Default constructor for VkTexture
     * @param device device which allocate from
     */
    VkTexture(vk::Device device);

    /** @brief Returns descriptor image info */
    const vk::DescriptorImageInfo& GetDescriptorImageInfo() const;

    /**
     * @brief Creates vulkan render texture
     * @param physicalDevice physical device for staging buffer
     * @param device device which allocate from
     * @param commandPool pool which allocate commands from
     * @param queue queue where push commands
     * @param texture texture data
     * @return true if creation was successful and false if not
     */
    bool Create(vk::PhysicalDevice const& physicalDevice, vk::Device const& device,
                vk::CommandPool const& commandPool, vk::Queue const& queue, Texture const& texture);

    /**
     * @brief Removes texture from GPU and destroys sampler
     */
    void Delete();

    /** @brief Returns @c true if texture is initialized and @c false otherwise */
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
