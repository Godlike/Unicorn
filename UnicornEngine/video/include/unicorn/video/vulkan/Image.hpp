/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_IMAGE_HPP
#define UNICORN_VIDEO_VULKAN_IMAGE_HPP

#include <vulkan/vulkan.hpp>
#include <unicorn/video/vulkan/Buffer.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
/**
 * @brief Wrapper for Vulkan image
 */
class Image
{
public:
    /**
   * @brief Creates image
   * @param device device for allocation
   * @param physicalDevice for memory type search and hardware allocation
   * @param format image specific format
   * @param usage image specific usage
   * @param width width of image
   * @param height height of image
   * @return true if creation sucessfull and false if not
   */
    Image(vk::PhysicalDevice physicalDevice,
        vk::Device device,
        vk::Format format,
        vk::ImageUsageFlags usage,
        uint32_t width,
        uint32_t height);
    /**
     * @brief Removes image
     */
    ~Image();

    Image(const Image& other) = delete;
    Image(Image&& other) = delete;
    Image& operator=(Image& other) = delete;
    Image& operator=(Image&& other) = delete;

    /**
     * @brief Checks if image was initialized before
     * @return true if was initialized before and false if not
     */
    bool IsInitialized() const;

    /**
     * @brief Returns image format
     * @return image format
     */
    vk::Format GetFormat() const;

    /**
     * @brief Returns image width
     * @return image width
     */
    int32_t GetWidth() const;

    /**
     * @brief Returns image height
     * @return image height
     */
    int32_t GetHeight() const;

    /**
     * @brief Returns vulkan raw image
     * @return vulkan raw image
     */
    const vk::Image& GetVkImage() const;

    /**
     * @brief Returns vulkan raw image view
     * @return vulkan raw image view
     */
    const vk::ImageView& GetVkImageView() const;
private:
    vk::Device m_device;
    vk::Image m_image;
    vk::ImageView m_imageView;
    Memory* m_deviceMemory;
    vk::Format m_format;
    vk::ImageUsageFlags m_usage;
    uint32_t m_width;
    uint32_t m_height;
    bool m_initialized;
};
}
}
}

#endif // UNICORN_VIDEO_VULKAN_IMAGE_HPP
