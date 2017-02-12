/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_VIDEO_RENDERER_HPP
#define VORPAL_VIDEO_RENDERER_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <vector>
#include <array>

#include <vorpal/system/Timer.hpp>
#include <vorpal/graphics/vulkan/VulkanBuffer.hpp>
#include <vorpal/graphics/vulkan/VulkanSwapChain.hpp>

struct GLFWwindow;

namespace vp
{
namespace graphics
{
class Vertex;

/** @brief  Vulkan renderer class
 *
 *  Initializes and controls Vulkan API
 */
class VulkanRenderer
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

    VulkanRenderer(const VulkanRenderer& other) = delete;
    VulkanRenderer(const VulkanRenderer&& other) = delete;
    VulkanRenderer& operator=(const VulkanRenderer& other) = delete;
    VulkanRenderer& operator=(const VulkanRenderer&& other) = delete;

    bool Init();
    void Deinit();
    void Render();
    bool RecreateSwapChain();

private:
    bool m_isInitialized;
    
    // TODO: make structs to peek this
    VulkanBuffer m_vertBuffer;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    VkBuffer m_uniformStagingBuffer;
    VkDeviceMemory m_uniformStagingBufferMemory;
    VkBuffer m_uniformBuffer;
    VkDeviceMemory m_uniformBufferMemory;
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_deviceExtensions;
    VkDebugReportCallbackEXT m_vulkanCallback;
    std::string m_gpuName;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;
    system::Timer m_timer;
    VkDescriptorSet m_descriptorSet;
#ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
#else
    static const bool s_enableValidationLayers = true;
#endif

    bool CreateInstance();
    bool PickPhysicalDevice();
    bool CreateLogicalDevice();
    bool CreateSurface();
    bool CreateSwapChain();
    bool CreateImageViews();
    bool CreateRenderPass();
    bool CreateGraphicsPipeline();
    bool CreateFramebuffers();
    bool CreateCommandPool();
    bool CreateDepthResources();
    bool CreateCommandBuffers();
    bool CreateSemaphores();
    bool CreateVertexBuffer();
    bool CreateIndexBuffer();
    bool CreateUniformBuffer();
    void UpdateUniformBuffer();
    bool CreateDescriptorSetLayout();
    bool CreateDescriptorPool();
    bool CreateDescriptorSet();
    bool LoadModel();
    bool CreateTextureImage();
    bool CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView);
    bool CreateTextureSampler();
    bool CreateTextureImageView();
    bool CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    bool HasStencilComponent(VkFormat format);
    bool CreateShaderModule(
        const std::vector<uint8_t>& code, VkShaderModule& shaderModule) const;
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool CreateBuffer(VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer& buffer,
        VkDeviceMemory& bufferMemory);
    bool CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    bool Frame();
    std::vector<const char*> GetRequiredExtensions();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    bool FindMemoryType(uint32_t typeFilter,
        VkMemoryPropertyFlags properties,
        uint32_t& memoryType);
    bool SetupDebugCallback();
    void DestroyDebugReportCallbackEXT() const;
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    bool TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
};
}
}

#endif // VORPAL_VIDEO_RENDERER_HPP
