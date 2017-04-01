/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT) 
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_RENDERER_HPP
#define UNICORN_VIDEO_RENDERER_HPP

#include <vulkan/vulkan.hpp>
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

    bool IsComplete() const
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
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
    static void OnWindowResized(GLFWwindow* window, int width, int height);
    bool RecreateSwapChain();

private:
    bool m_isInitialized;
    GLFWwindow* m_pWindow;
    vk::Instance m_vkInstance;
    vk::PhysicalDevice m_vkPhysicalDevice;
    vk::Device m_vkLogicalDevice;
    vk::SwapchainKHR m_vkSwapChain;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;
    vk::SurfaceKHR m_vkWindowSurface;
    vk::Format m_swapChainImageFormat;
    vk::Extent2D m_swapChainExtent;
    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_graphicsPipeline;
    vk::RenderPass m_renderPass;
    vk::CommandPool m_commandPool;
    vk::Semaphore m_imageAvailableSemaphore;
    vk::Semaphore m_renderFinishedSemaphore;
    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_deviceExtensions;
    VkDebugReportCallbackEXT m_vulkanCallback;
    std::string m_gpuName;
    std::vector<vk::Image> m_swapChainImages;
    std::vector<vk::ImageView> m_swapChainImageViews;
    std::vector<vk::Framebuffer> m_swapChainFramebuffers;
    std::vector<vk::CommandBuffer> m_commandBuffers;
#ifdef NDEBUG
    static const bool s_enableValidationLayers = false;
#else
    static const bool s_enableValidationLayers = true;
#endif

    void FreeInstance();
    void FreeDebugCallback();
    void FreeSurface();
    void FreeLogicalDevice();
    void FreeSwapChain();
    void FreeImageViews();
    void FreeRenderPass();
    void FreeGraphicsPipeline();
    void FreePipelineLayout();
    void FreeFramebuffers();
    void FreeCommandPool();
    void FreeCommandBuffers();
    void FreeSemaphores();

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
    bool CreateShaderModule(const std::vector<uint8_t>& code, vk::ShaderModule& shaderModule);
    bool IsDeviceSuitable(const vk::PhysicalDevice& device);
    bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device);
    bool Frame();
    std::vector<const char*> GetRequiredExtensions();
    QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device);
    bool QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device);
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
    bool SetupDebugCallback();
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo);
    void DestroyDebugReportCallbackEXT();
};
}
}

#endif // UNICORN_VIDEO_RENDERER_HPP
