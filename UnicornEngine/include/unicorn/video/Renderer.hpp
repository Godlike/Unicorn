/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <vulkan/vulkan.hpp>
//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <string>

struct GLFWwindow;

namespace unicorn
{
namespace video
{
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
class Renderer
{
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer(const Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(const Renderer&& other) = delete;

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
    VkPipeline m_graphicsPipeline;
    VkRenderPass m_renderPass;
    VkCommandPool m_commandPool;
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
    bool CreateCommandBuffers();
    bool CreateSemaphores();
    bool CreateShaderModule(const std::vector<uint8_t>& code, VkShaderModule& shaderModule);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    bool Frame();
    std::vector<const char*> GetRequiredExtensions();
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    bool SetupDebugCallback();
    void DestroyDebugReportCallbackEXT();
    void waitAsyncEnd();
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
