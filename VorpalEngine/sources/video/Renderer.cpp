/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/core/Settings.hpp>
#include <vorpal/utility/Logger.hpp>
#include <vorpal/utility/asset/SimpleStorage.hpp>
#include <vorpal/video/Renderer.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vp
{
namespace video
{
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char *layerPrefix,
    const char *msg,
    void *userData)
{
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("VULKAN LAYER ERROR: %s", msg);
    };
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARNING("VULKAN LAYER WARNING: %s", msg);
    };
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER PERFORMANCE: %s", msg);
    };
    return VK_FALSE;
}

Renderer::Renderer()
    : m_isInitialized(false)
    , m_pWindow(nullptr)
    , m_vkInstance(VK_NULL_HANDLE)
    , m_vkPhysicalDevice(VK_NULL_HANDLE)
    , m_validationLayers({"VK_LAYER_LUNARG_standard_validation"})
    , m_deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME})
{
    m_vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};
    m_indices = {0, 1, 2, 2, 3, 0};

    m_vkSwapChain = VK_NULL_HANDLE;
    m_renderPass = VK_NULL_HANDLE;
    m_pipelineLayout = VK_NULL_HANDLE;
    m_graphicsPipeline = VK_NULL_HANDLE;
    m_vkLogicalDevice = VK_NULL_HANDLE;
    m_vulkanCallback = VK_NULL_HANDLE;
    m_imageAvailableSemaphore = VK_NULL_HANDLE;
    m_renderFinishedSemaphore = VK_NULL_HANDLE;
    m_commandPool = VK_NULL_HANDLE;
    m_vkWindowSurface = VK_NULL_HANDLE;
    m_vertexBuffer = VK_NULL_HANDLE;
    m_vertexBufferMemory = VK_NULL_HANDLE;
    m_indexBuffer = VK_NULL_HANDLE;
    m_indexBufferMemory = VK_NULL_HANDLE;
    m_descriptorSetLayout = VK_NULL_HANDLE;
    m_uniformStagingBufferMemory = VK_NULL_HANDLE;
    m_uniformStagingBuffer = VK_NULL_HANDLE;
    m_uniformBufferMemory = VK_NULL_HANDLE;
    m_uniformBuffer = VK_NULL_HANDLE;
}

Renderer::~Renderer()
{
    Deinit();
}

bool Renderer::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Vulkan render initialization started.");

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if (!glfwVulkanSupported())
    {
        LOG_ERROR("Vulkan not supported!");

        return false;
    }

    const core::Settings &settings = core::Settings::Instance();

    m_pWindow = glfwCreateWindow(settings.GetApplicationWidth(),
        settings.GetApplicationHeight(),
        settings.GetApplicationName().c_str(),
        nullptr,
        nullptr);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowSizeCallback(m_pWindow, onWindowResized);
    if (!CreateInstance() || !SetupDebugCallback() || !CreateSurface() ||
        !PickPhysicalDevice() || !CreateLogicalDevice() || !CreateSwapChain() ||
        !CreateImageViews() || !CreateRenderPass() ||
        !CreateDescriptorSetLayout() || !CreateGraphicsPipeline() ||
        !CreateFramebuffers() || !CreateCommandPool() ||
        !CreateVertexBuffer() || !CreateIndexBuffer() ||
        !CreateUniformBuffer() || !CreateDescriptorPool() ||
        !CreateDescriptorSet() || !CreateCommandBuffers() ||
        !CreateSemaphores())
        return false;
    m_timer.Start();
    m_isInitialized = true;

    LOG_INFO("Vulkan render initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    if (m_uniformStagingBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_uniformStagingBufferMemory, nullptr);
        m_uniformStagingBufferMemory = VK_NULL_HANDLE;
    }

    if (m_uniformStagingBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_vkLogicalDevice, m_uniformStagingBuffer, nullptr);
        m_uniformStagingBuffer = VK_NULL_HANDLE;
    }
    if (m_uniformBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_uniformBufferMemory, nullptr);
        m_uniformBufferMemory = VK_NULL_HANDLE;
    }

    if (m_uniformBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_vkLogicalDevice, m_uniformBuffer, nullptr);
        m_uniformBuffer = VK_NULL_HANDLE;
    }

    if (m_descriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(
            m_vkLogicalDevice, m_descriptorSetLayout, nullptr);
        m_descriptorSetLayout = VK_NULL_HANDLE;
    }
    if (m_descriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(m_vkLogicalDevice, m_descriptorPool, nullptr);
        m_descriptorPool = VK_NULL_HANDLE;
    }

    if (m_indexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_indexBufferMemory, nullptr);
        m_indexBufferMemory = VK_NULL_HANDLE;
    }

    if (m_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_vkLogicalDevice, m_indexBuffer, nullptr);
        m_indexBuffer = VK_NULL_HANDLE;
    }

    if (m_vertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_vertexBufferMemory, nullptr);
        m_vertexBufferMemory = VK_NULL_HANDLE;
    }

    if (m_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_vkLogicalDevice, m_vertexBuffer, nullptr);
        m_vertexBuffer = VK_NULL_HANDLE;
    }

    if (m_vulkanCallback != VK_NULL_HANDLE)
    {
        DestroyDebugReportCallbackEXT();
        m_vulkanCallback = VK_NULL_HANDLE;
    }

    for (VkImageView &view : m_swapChainImageViews)
    {
        vkDestroyImageView(m_vkLogicalDevice, view, nullptr);
        view = VK_NULL_HANDLE;
    }
    m_swapChainImageViews.clear();

    for (VkFramebuffer &framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_vkLogicalDevice, framebuffer, nullptr);
        framebuffer = VK_NULL_HANDLE;
    }
    m_swapChainFramebuffers.clear();

    if (m_imageAvailableSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(
            m_vkLogicalDevice, m_imageAvailableSemaphore, nullptr);
        m_imageAvailableSemaphore = VK_NULL_HANDLE;
    }

    if (m_renderFinishedSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(
            m_vkLogicalDevice, m_renderFinishedSemaphore, nullptr);
        m_renderFinishedSemaphore = VK_NULL_HANDLE;
    }

    if (m_pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(m_vkLogicalDevice, m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }

    if (m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_vkLogicalDevice, m_commandPool, nullptr);
        m_commandPool = VK_NULL_HANDLE;
    }

    if (m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(m_vkLogicalDevice, m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }

    if (m_graphicsPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_vkLogicalDevice, m_graphicsPipeline, nullptr);
        m_graphicsPipeline = VK_NULL_HANDLE;
    }

    if (m_vkSwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_vkLogicalDevice, m_vkSwapChain, nullptr);
        m_vkSwapChain = VK_NULL_HANDLE;
    }

    if (m_vkLogicalDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_vkLogicalDevice, nullptr);
        m_vkLogicalDevice = VK_NULL_HANDLE;
    }

    if (m_vkWindowSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_vkInstance, m_vkWindowSurface, nullptr);
        m_vkWindowSurface = VK_NULL_HANDLE;
    }

    // Instance must be freed last but before glfw window.
    if (m_vkInstance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_vkInstance, nullptr);
        m_vkInstance = VK_NULL_HANDLE;
    }

    if (m_pWindow)
    {
        glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
    if (!m_isInitialized)
        LOG_INFO("Vulkan render shutdown correctly.");
    m_isInitialized = false;
}

QueueFamilyIndices Renderer::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 &&
            queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        //@todo optimize?
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            device, i, m_vkWindowSurface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }
        ++i;
    }

    return indices;
}

SwapChainSupportDetails Renderer::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device, m_vkWindowSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, m_vkWindowSurface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, m_vkWindowSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, m_vkWindowSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,
            m_vkWindowSurface,
            &presentModeCount,
            details.presentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 &&
        availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR Renderer::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities)
{
    const core::Settings &settings = core::Settings::Instance();

    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {
            settings.GetApplicationWidth(), settings.GetApplicationHeight()};

        actualExtent.width = std::max(capabilities.minImageExtent.width,
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

bool Renderer::FindMemoryType(
    uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t &memoryType)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) ==
                properties)
        {
            memoryType = i;
            return true;
        }
    }
    LOG_ERROR("Failed to find suitable memory type!");
    return false;
}

VkResult Renderer::CreateDebugReportCallbackEXT(
    const VkDebugReportCallbackCreateInfoEXT *pCreateInfo)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugReportCallbackEXT"));
    if (func != nullptr)
    {
        return func(m_vkInstance, pCreateInfo, nullptr, &m_vulkanCallback);
    }
    else
    {
        LOG_ERROR("Can't setup debug callback'");
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Renderer::DestroyDebugReportCallbackEXT()
{
    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT"));
    if (func != nullptr)
    {
        func(m_vkInstance, m_vulkanCallback, nullptr);
    }
}

void Renderer::waitAsyncEnd()
{
    vkDeviceWaitIdle(m_vkLogicalDevice);
}

void Renderer::Render()
{
    if (m_isInitialized && m_pWindow)
    {
        double lastTime = glfwGetTime();
        int nbFrames = 0;
        while (!glfwWindowShouldClose(m_pWindow))
        {
#ifndef NDEBUG
            double currentTime = glfwGetTime();
            nbFrames++;
            if (currentTime - lastTime >= 1.0)
            {
                std::string buf(
                    core::Settings::Instance().GetApplicationName().c_str());
                buf.append(" MS/FPS ");
                buf.append(std::to_string(1000.0 / double(nbFrames)).c_str());
                buf.append(" FPS ");
                buf.append(std::to_string(double(nbFrames)).c_str());
                glfwSetWindowTitle(m_pWindow, buf.c_str());

                nbFrames = 0;
                lastTime += 1.0;
            }
#endif
            glfwPollEvents();
            UpdateUniformBuffer();
            if (!Frame())
                break;
        }
        waitAsyncEnd();
    }
}

void Renderer::onWindowResized(GLFWwindow *window, int width, int height)
{
    if (width == 0 || height == 0)
        return;
    Renderer *renderer =
        reinterpret_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (!renderer->RecreateSwapChain())
        LOG_ERROR("Can't recreate swapchain!");
}

bool Renderer::RecreateSwapChain()
{
    waitAsyncEnd();

    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFramebuffers();
    CreateCommandBuffers();
    return true;
}

bool Renderer::CreateInstance()
{
    if (s_enableValidationLayers && !CheckValidationLayerSupport())
    {
        LOG_ERROR("Vulkan validation layers requested, but not available!");
        return false;
    }

    const core::Settings &settings = core::Settings::Instance();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = settings.GetApplicationName().c_str();
    appInfo.applicationVersion =
        VK_MAKE_VERSION(1, 0, 0);  // TODO: remove hardcode
    appInfo.pEngineName = settings.GetVorpalEngineName().c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);  // TODO: remove hardcode
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = GetRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (s_enableValidationLayers)
    {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create instance!");

        m_vkInstance = VK_NULL_HANDLE;

        return false;
    }

    return true;
}

bool Renderer::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        LOG_ERROR("Failed to find GPUs with Vulkan support!");
        return false;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        if (IsDeviceSuitable(device))
        {
            m_vkPhysicalDevice = device;
            break;
        }
    }

    if (m_vkPhysicalDevice == VK_NULL_HANDLE)
    {
        LOG_ERROR("Failed to find a suitable GPU!");
        return false;
    }

    return true;
}

bool Renderer::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {
        indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};  // No features for now.

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount =
        static_cast<uint32_t>(m_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    if (s_enableValidationLayers)
    {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    if (vkCreateDevice(
            m_vkPhysicalDevice, &createInfo, nullptr, &m_vkLogicalDevice) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Can't initialize Vulkan logical device!");
        return false;
    }
    vkGetDeviceQueue(
        m_vkLogicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
    vkGetDeviceQueue(
        m_vkLogicalDevice, indices.presentFamily, 0, &m_presentQueue);

    return true;
}

bool Renderer::CreateSurface()
{
    if (glfwCreateWindowSurface(
            m_vkInstance, m_pWindow, nullptr, &m_vkWindowSurface) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create window surface!");
        return false;
    }
    return true;
}

bool Renderer::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport =
        QuerySwapChainSupport(m_vkPhysicalDevice);
    VkSurfaceFormatKHR surfaceFormat =
        ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =
        ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
        imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_vkWindowSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;  // VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                              // for post processing.

    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);
    uint32_t queueFamilyIndices[] = {
        (uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    VkSwapchainKHR oldSwapChain = m_vkSwapChain;
    if (oldSwapChain != VK_NULL_HANDLE)
        createInfo.oldSwapchain = oldSwapChain;
    VkSwapchainKHR newSwapChain;
    if (vkCreateSwapchainKHR(
            m_vkLogicalDevice, &createInfo, nullptr, &newSwapChain) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create Vulkan swap chain!");
    }
    m_vkSwapChain = newSwapChain;

    if (oldSwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_vkLogicalDevice, oldSwapChain, nullptr);
        oldSwapChain = VK_NULL_HANDLE;
    }

    vkGetSwapchainImagesKHR(
        m_vkLogicalDevice, m_vkSwapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_vkLogicalDevice,
        m_vkSwapChain,
        &imageCount,
        m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    return true;
}

bool Renderer::CreateImageViews()
{
    for (VkImageView &view : m_swapChainImageViews)
    {
        vkDestroyImageView(m_vkLogicalDevice, view, nullptr);
    }
    m_swapChainImageViews.resize(m_swapChainImages.size());
    for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(m_vkLogicalDevice,
                &createInfo,
                nullptr,
                &m_swapChainImageViews[i]) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to create image views!");
            return false;
        }
    }
    return true;
}

bool Renderer::CreateRenderPass()
{
    if (m_renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(m_vkLogicalDevice, m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(
            m_vkLogicalDevice, &renderPassInfo, nullptr, &m_renderPass) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create render pass!");
        return false;
    }

    return true;
}

bool Renderer::CreateGraphicsPipeline()
{
    if (m_pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(m_vkLogicalDevice, m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }
    if (m_graphicsPipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(m_vkLogicalDevice, m_graphicsPipeline, nullptr);
        m_graphicsPipeline = VK_NULL_HANDLE;
    }

    vp::utility::asset::SimpleStorage &storage =
        vp::utility::asset::SimpleStorage::Instance();
    vp::utility::asset::Handler simpleVertShaderHandler =
        storage.Get("data/shaders/vert.spv");
    vp::utility::asset::Handler simpleFragShaderHandler =
        storage.Get("data/shaders/frag.spv");
    if (!simpleVertShaderHandler.IsValid() ||
        !simpleFragShaderHandler.IsValid())
    {
        LOG_ERROR("Can't find shaders!");
        return false;
    }

    VkShaderModule vertShaderModule = {};
    VkShaderModule fragShaderModule = {};
    bool shadersCreatedFailed =
        !CreateShaderModule(simpleVertShaderHandler.GetContent().GetBuffer(),
            vertShaderModule) ||
        !CreateShaderModule(
            simpleFragShaderHandler.GetContent().GetBuffer(), fragShaderModule);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        vertShaderStageInfo, fragShaderStageInfo};

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport =
        {};  // TODO : if we will using super sampling. Need to rethink.
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    VkPipelineMultisampleStateCreateInfo multisampling =
        {};  // TODO: configure MSAA
    multisampling.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // For future VkPipelineDepthStencilStateCreateInfo

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDescriptorSetLayout setLayouts[] = {m_descriptorSetLayout};
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = setLayouts;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = 0;

    if (vkCreatePipelineLayout(m_vkLogicalDevice,
            &pipelineLayoutInfo,
            nullptr,
            &m_pipelineLayout) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create pipeline layout!");
        return false;
    }

    if (shadersCreatedFailed)
    {
        LOG_ERROR("Can't create shader module!");
        return false;
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;  // Optional

    if (vkCreateGraphicsPipelines(m_vkLogicalDevice,
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &m_graphicsPipeline) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create graphics pipeline!");
        return false;
    }

    vkDestroyShaderModule(m_vkLogicalDevice, vertShaderModule, nullptr);
    vkDestroyShaderModule(m_vkLogicalDevice, fragShaderModule, nullptr);

    return true;
}

bool Renderer::CreateFramebuffers()
{
    for (VkFramebuffer &framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_vkLogicalDevice, framebuffer, nullptr);
    }
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());
    for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
    {
        VkImageView attachments[] = {m_swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_vkLogicalDevice,
                &framebufferInfo,
                nullptr,
                &m_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to create framebuffer!");
            return false;
        }
    }
    return true;
}

bool Renderer::CreateCommandPool()
{
    QueueFamilyIndices queueFamilyIndices =
        FindQueueFamilies(m_vkPhysicalDevice);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags = 0;

    if (vkCreateCommandPool(
            m_vkLogicalDevice, &poolInfo, nullptr, &m_commandPool) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create command pool!");
        return false;
    }

    return true;
}

bool Renderer::CreateCommandBuffers()
{
    if (m_commandBuffers.size() > 0)
    {
        vkFreeCommandBuffers(m_vkLogicalDevice,
            m_commandPool,
            static_cast<uint32_t>(m_commandBuffers.size()),
            m_commandBuffers.data());
    }
    m_commandBuffers.resize(m_swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_vkLogicalDevice,
            &allocInfo,
            m_commandBuffers.data()) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to allocate command buffers!");
        return false;
    }

    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

        vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapChainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(
            m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(m_commandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_graphicsPipeline);

        VkBuffer vertexBuffers[] = {m_vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(
            m_commandBuffers[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(m_commandBuffers[i],
            m_indexBuffer,
            0,
            VK_INDEX_TYPE_UINT16);  // Be careful here, don't forget to change
                                    // UINTxxx
        vkCmdBindDescriptorSets(m_commandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout,
            0,
            1,
            &m_descriptorSet,
            0,
            nullptr);
        vkCmdDrawIndexed(m_commandBuffers[i], m_indices.size(), 1, 0, 0, 0);
        vkCmdEndRenderPass(m_commandBuffers[i]);

        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to record command buffer!");
            return false;
        }
    }
    return true;
}

bool Renderer::CreateSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(m_vkLogicalDevice,
            &semaphoreInfo,
            nullptr,
            &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(m_vkLogicalDevice,
            &semaphoreInfo,
            nullptr,
            &m_renderFinishedSemaphore) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create semaphores!");
        return false;
    }
    return true;
}

bool Renderer::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory))
        return false;
    void *data;

    vkMapMemory(
        m_vkLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_vkLogicalDevice, stagingBufferMemory);

    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_vertexBuffer,
            m_vertexBufferMemory))
        return false;
    if (!CopyBuffer(stagingBuffer, m_vertexBuffer, bufferSize))
        return false;
    vkFreeMemory(m_vkLogicalDevice, stagingBufferMemory, nullptr);
    vkDestroyBuffer(m_vkLogicalDevice, stagingBuffer, nullptr);
    return true;
}

bool Renderer::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory))
        return false;
    void *data;
    vkMapMemory(
        m_vkLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_indices.data(), (size_t) bufferSize);
    vkUnmapMemory(m_vkLogicalDevice, stagingBufferMemory);

    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_indexBuffer,
            m_indexBufferMemory))
        return false;

    if (!CopyBuffer(stagingBuffer, m_indexBuffer, bufferSize))
        return false;
    vkFreeMemory(m_vkLogicalDevice, stagingBufferMemory, nullptr);
    vkDestroyBuffer(m_vkLogicalDevice, stagingBuffer, nullptr);
    return true;
}

bool Renderer::CreateUniformBuffer()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_uniformStagingBuffer,
            m_uniformStagingBufferMemory))
        return false;
    if (!CreateBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_uniformBuffer,
            m_uniformBufferMemory))
        return false;
    return true;
}

void Renderer::UpdateUniformBuffer()
{
    auto time = m_timer.Seconds();
    UniformBufferObject ubo = {};
    ubo.model = glm::rotate(
        glm::mat4(), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f),
        m_swapChainExtent.width / (float) m_swapChainExtent.height,
        0.1f,
        10.0f);
    ubo.proj[1][1] *= -1;

    void *data;
    vkMapMemory(m_vkLogicalDevice,
        m_uniformStagingBufferMemory,
        0,
        sizeof(ubo),
        0,
        &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(m_vkLogicalDevice, m_uniformStagingBufferMemory);

    CopyBuffer(m_uniformStagingBuffer, m_uniformBuffer, sizeof(ubo));
}

bool Renderer::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(
            m_vkLogicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout) !=
        VK_SUCCESS)
    {
        LOG_ERROR("failed to create descriptor set layout!");
        return false;
    }
    return true;
}

bool Renderer::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(
            m_vkLogicalDevice, &poolInfo, nullptr, &m_descriptorPool) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create descriptor pool!");
        return false;
    }
    return true;
}

bool Renderer::CreateDescriptorSet()
{
    VkDescriptorSetLayout layouts[] = {m_descriptorSetLayout};
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts;

    if (vkAllocateDescriptorSets(
            m_vkLogicalDevice, &allocInfo, &m_descriptorSet) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to allocate descriptor set!");
        return false;
    }
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = m_uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = m_descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;        // Optional
    descriptorWrite.pTexelBufferView = nullptr;  // Optional

    vkUpdateDescriptorSets(m_vkLogicalDevice, 1, &descriptorWrite, 0, nullptr);

    return true;
}

bool Renderer::CreateShaderModule(
    const std::vector<uint8_t> &code, VkShaderModule &shaderModule)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = (uint32_t *) code.data();
    if (vkCreateShaderModule(
            m_vkLogicalDevice, &createInfo, nullptr, &shaderModule) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create shader module!");
        return false;
    }
    return true;
}

bool Renderer::IsDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    LOG_INFO("Found GPU : %s", deviceProperties.deviceName);
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAcceptable = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport =
            QuerySwapChainSupport(device);
        swapChainAcceptable = !swapChainSupport.formats.empty() &&
            !swapChainSupport.presentModes.empty();
    }
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        indices.isComplete() && extensionsSupported && swapChainAcceptable)
    {
        LOG_INFO("Picked as main GPU : %s", deviceProperties.deviceName);
        m_gpuName = deviceProperties.deviceName;
        return true;
    }
    else
    {
        return false;
    }
}

bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(
        m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Renderer::CreateBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer &buffer,
    VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_vkLogicalDevice, &bufferInfo, nullptr, &buffer) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create buffer!");
        return false;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_vkLogicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    if (!FindMemoryType(memRequirements.memoryTypeBits,
            properties,
            allocInfo.memoryTypeIndex))
        return false;

    if (vkAllocateMemory(
            m_vkLogicalDevice, &allocInfo, nullptr, &bufferMemory) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to allocate buffer memory!");
        return false;
    }

    vkBindBufferMemory(m_vkLogicalDevice, buffer, bufferMemory, 0);
    return true;
}

bool Renderer::CopyBuffer(
    VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;  // TODO: look here, maybe we need to
                                            // create temporary commandPool?
                                            // Maybe inplace? Just profile it.
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_vkLogicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);
    vkFreeCommandBuffers(m_vkLogicalDevice, m_commandPool, 1, &commandBuffer);
    return true;
}

bool Renderer::Frame()
{
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_vkLogicalDevice,
        m_vkSwapChain,
        std::numeric_limits<uint64_t>::max(),
        m_imageAvailableSemaphore,
        VK_NULL_HANDLE,
        &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain();
        return true;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to submit draw command buffer!");
        return false;
    }
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(m_presentQueue,
        &presentInfo);  // TODO : some error here if no vertex data provided?
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain();
        return true;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }
    return true;
}

bool Renderer::CheckValidationLayerSupport() const
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : m_validationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            LOG_ERROR("Can't find required Vulkan layers: ");

            for (auto &requiredLayer : m_validationLayers)
            {
                LOG_ERROR(requiredLayer);
            }

            return false;
        }
    }

    return true;
}

std::vector<const char *> Renderer::GetRequiredExtensions()
{
    std::vector<const char *> m_extensions;
    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        m_extensions.push_back(glfwExtensions[i]);
    }

    if (s_enableValidationLayers)
    {
        m_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return m_extensions;
}

bool Renderer::SetupDebugCallback()
{
    if (!s_enableValidationLayers)
        return true;

    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
        VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = DebugCallback;
    return CreateDebugReportCallbackEXT(&createInfo) == VK_SUCCESS;
}
}
}
