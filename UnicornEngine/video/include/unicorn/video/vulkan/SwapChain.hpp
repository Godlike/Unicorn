/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_SWAPCHAIN_HPP
#define UNICORN_VIDEO_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
class Swapchain
{
public:
    Swapchain();
    ~Swapchain();

    vk::Result Create(
        vk::PhysicalDevice physicalDevice,
        vk::Device device,
        vk::SurfaceKHR& surface,
        vk::ImageUsageFlags usage,
        uint32_t width,
        uint32_t height);
    void Destroy();

    vk::ImageUsageFlags GetUsage();
    vk::Format GetFormat();
    uint32_t GetWidth();
    uint32_t GetHeight();
    size_t Count();

    vk::SwapchainKHR& GetVkSwapchainKHR();
    vk::Image& GetVkImage(size_t index);
    vk::ImageView& GetVkImageView(size_t index);
private:
    vk::Device m_device;
    vk::ImageUsageFlags m_usage;
    vk::Format m_format;
    uint32_t m_width;
    uint32_t m_height;
    vk::SwapchainKHR m_swapchain;
    std::vector<vk::Image> m_images;
    std::vector<vk::ImageView> m_imageViews;
};
}
}
}

#endif // UNICORN_VIDEO_SWAPCHAIN_HPP