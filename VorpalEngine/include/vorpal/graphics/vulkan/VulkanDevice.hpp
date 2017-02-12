/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_VULKAN_DEVICE_HPP
#define VORPAL_GRAPHICS_VULKAN_DEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace vp
{
    namespace system 
    {
        class Window;
    }
namespace graphics 
{
    class VulkanDevice 
    {
    public:
        void Init(const system::Window& renderWindow);
        void Deinit();
        VulkanDevice();

        ~VulkanDevice();

        VulkanDevice(const VulkanDevice& other) = delete;
        VulkanDevice(const VulkanDevice&& other) = delete;
        VulkanDevice& operator=(const VulkanDevice& other) = delete;
        VulkanDevice& operator=(const VulkanDevice&& other) = delete;
    private:
        vk::Instance m_vkInstance;
        VkPhysicalDevice m_vkPhysicalDevice;
        VkDevice m_vkLogicalDevice;
        VkQueue m_graphicsQueue;
        VkSurfaceKHR m_vkWindowSurface;
        VkPipelineLayout m_pipelineLayout;
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkPipeline m_graphicsPipeline;
        VkRenderPass m_renderPass;
    };
}
}

#endif // VORPAL_GRAPHICS_VULKAN_DEVICE_HPP