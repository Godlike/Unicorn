/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_VULKANSWAPCHAIN_HPP
#define VORPAL_GRAPHICS_VULKANSWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace vp
{
namespace graphics
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanSwapChain
    {
    private:
        VkSwapchainKHR m_vkSwapChain;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
    };
}
}
#endif // VORPAL_GRAPHICS_VULKANSWAPCHAIN_HPP
