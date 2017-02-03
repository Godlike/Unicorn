/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_VIDEO_RENDERER_HPP
#define VORPAL_VIDEO_RENDERER_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>
#include <array>

#include <vorpal/system/Timer.hpp>

struct GLFWwindow;

namespace vp
{
namespace graphics

{
struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() const
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

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
    static void onWindowResized(GLFWwindow* window, int width, int height);
    bool RecreateSwapChain();

private:
    bool m_isInitialized;
    GLFWwindow* m_pWindow;

    VkInstance m_vkInstance;
    VkPhysicalDevice m_vkPhysicalDevice;
    VkDevice m_vkLogicalDevice;
    VkSwapchainKHR m_vkSwapChain;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkSurfaceKHR m_vkWindowSurface;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    VkPipelineLayout m_pipelineLayout;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipeline m_graphicsPipeline;
    VkRenderPass m_renderPass;
    VkCommandPool m_commandPool;
    // TODO: make structs to peek this
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    VkDescriptorPool m_descriptorPool;
    VkBuffer m_uniformStagingBuffer;
    VkDeviceMemory m_uniformStagingBufferMemory;
    VkBuffer m_uniformBuffer;
    VkDeviceMemory m_uniformBufferMemory;
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;
    const std::string m_modelPath = "data/models/cabin/chalet.obj";
    const std::string m_texturePath = "data/models/cabin/chalet.jpg";

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
    bool CheckValidationLayerSupport() const;
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
    void waitAsyncEnd() const;
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
};
}
}

#endif // VORPAL_VIDEO_RENDERER_HPP
