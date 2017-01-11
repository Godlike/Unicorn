#include <vorpal/video/Renderer.hpp>
#include <vorpal/core/Settings.hpp>
#include <vorpal/utility/Logger.hpp>

#include <cstring>
#include <iostream>
#include <set>
#include <algorithm>

namespace vp
{
namespace video
{

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
                                                    VkDebugReportObjectTypeEXT objType, 
                                                    uint64_t obj, 
                                                    size_t location, 
                                                    int32_t code, 
                                                    const char* layerPrefix, 
                                                    const char* msg, 
                                                    void* userData) {
    // Error that may result in undefined behaviour
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("VULKAN LAYER ERROR: %s",  msg);  
    };
    // Warnings may hint at unexpected / non-spec API usage
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARNING("VULKAN LAYER WARNING: %s",  msg);        
    };
    // May indicate sub-optimal usage of the API
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER PERFORMANCE: %s",  msg);
    };
    // Informal messages that may become handy during debugging
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER INFO: %s",  msg);
    }
    // Diagnostic info from the Vulkan loader and layers
    // Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
        LOG_DEBUG("VULKAN LAYER DEBUG: %s",  msg);
    }
    return VK_FALSE;
}

Renderer::Renderer()
    : m_isInitialized(false)
    , m_pWindow(nullptr)
    , m_vkInstance(VK_NULL_HANDLE)
    , m_vkPhysicalDevice(VK_NULL_HANDLE)
    , m_validationLayers( {"VK_LAYER_LUNARG_standard_validation"} )
    , m_deviceExtensions( {VK_KHR_SWAPCHAIN_EXTENSION_NAME})
{
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (!glfwVulkanSupported())
    {
        LOG_ERROR("Vulkan not supported!");

        return false;
    }

    const core::Settings& settings = core::Settings::Instance();

    m_pWindow = glfwCreateWindow(
        settings.GetApplicationWidth(),
        settings.GetApplicationHeight(),
        settings.GetApplicationName().c_str(),
        nullptr, nullptr);
    if (!CreateInstance()
        || !SetupDebugCallback()
        || !CreateSurface()
        || !PickPhysicalDevice()
        || !CreateLogicalDevice()
        || !CreateSwapChain()
        || !CreateImageViews())
    return false;

    m_isInitialized = true;

    LOG_INFO("Vulkan render initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    if(m_vulkanCallback != VK_NULL_HANDLE)
    {
        DestroyDebugReportCallbackEXT();
    }

    for(VkImageView& view: m_swapChainImageViews)
    {
        vkDestroyImageView(m_vkLogicalDevice, view, nullptr);
    }
    m_swapChainImageViews.clear();

    if(m_vkSwapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_vkLogicalDevice, m_vkSwapChain, nullptr);
        m_vkSwapChain = VK_NULL_HANDLE;
    }

    if(m_vkLogicalDevice!= VK_NULL_HANDLE) {
        vkDestroyDevice(m_vkLogicalDevice, nullptr);
        m_vkLogicalDevice = VK_NULL_HANDLE;
    }

    if(m_vkWindowSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_vkInstance ,m_vkWindowSurface, nullptr);
        m_vkWindowSurface = VK_NULL_HANDLE;
    }    

    //Instance must be freed last but before glfw window.
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

    m_isInitialized = false;
    LOG_INFO("Vulkan render shutdown correctly.");
}

QueueFamilyIndices Renderer::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        //TODO optimize?
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_vkWindowSurface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport) {
             indices.presentFamily = i;
         }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

SwapChainSupportDetails Renderer::QuerySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_vkWindowSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkWindowSurface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkWindowSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkWindowSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkWindowSurface, &presentModeCount, details.presentModes.data());
    }
    return details;
}

VkSurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
           return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
       }

       for (const auto& availableFormat : availableFormats) {
           if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
               return availableFormat;
           }
       }

       return availableFormats[0];
}

VkPresentModeKHR Renderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
           if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
               return availablePresentMode;
           }
       }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    const core::Settings& settings = core::Settings::Instance();

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {settings.GetApplicationWidth(),
                                   settings.GetApplicationHeight()};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

VkResult Renderer::CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo) {
    auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugReportCallbackEXT"));
    if (func != nullptr) {
        return func(m_vkInstance, pCreateInfo, nullptr, &m_vulkanCallback);
    } else {        
        LOG_ERROR("Can't setup debug callback'");
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Renderer::DestroyDebugReportCallbackEXT() {
    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT"));
    if (func != nullptr) {
        func(m_vkInstance, m_vulkanCallback, nullptr);
    }
}

void Renderer::Render()
{
    if (m_isInitialized && m_pWindow)
    {
        while (!glfwWindowShouldClose(m_pWindow))
        {
            glfwPollEvents();
        }
    }
}

bool Renderer::CreateInstance()
{
    if (s_enableValidationLayers && !CheckValidationLayerSupport())
    {
        LOG_ERROR("Vulkan validation layers requested, but not available!");
        return false;
    }

    const core::Settings& settings = core::Settings::Instance();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = settings.GetApplicationName().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: remove hardcode
    appInfo.pEngineName = settings.GetVorpalEngineName().c_str();
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0); // TODO: remove hardcode
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = GetRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (s_enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
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

    if (deviceCount == 0) {
        LOG_ERROR("failed to find GPUs with Vulkan support!");
        return false;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            m_vkPhysicalDevice = device;
            break;
        }
    }

    if (m_vkPhysicalDevice == VK_NULL_HANDLE) {
        LOG_ERROR("failed to find a suitable GPU!");
        return false;
    }

    return true;
}

bool Renderer::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {}; // No features for now.

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = m_deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    if (s_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        createInfo.ppEnabledLayerNames = m_validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    if (vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkLogicalDevice) != VK_SUCCESS) {
        LOG_ERROR("Can't initialize Vulkan logical device!");
        return false;
    }
    vkGetDeviceQueue(m_vkLogicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_vkLogicalDevice, indices.presentFamily, 0, &m_presentQueue);

    return true;
}

bool Renderer::CreateSurface()
{
    if (glfwCreateWindowSurface(m_vkInstance, m_pWindow, nullptr, &m_vkWindowSurface) != VK_SUCCESS) {
           LOG_ERROR("Failed to create window surface!");
           return false;
    }
    return true;
}

bool Renderer::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_vkPhysicalDevice);
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
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
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //VK_IMAGE_USAGE_TRANSFER_DST_BIT for post processing.

    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);
    uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(m_vkLogicalDevice, &createInfo, nullptr, &m_vkSwapChain) != VK_SUCCESS) {
        LOG_ERROR("Failed to create Vulkan swap chain!");
    }

    vkGetSwapchainImagesKHR(m_vkLogicalDevice, m_vkSwapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_vkLogicalDevice, m_vkSwapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    return true;
}

bool Renderer::CreateImageViews()
{
     m_swapChainImageViews.resize(m_swapChainImages.size());
     for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
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
         if (vkCreateImageView(m_vkLogicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
             LOG_ERROR("Failed to create image views!");
             return false;
         }
     }
     return true;
}

bool Renderer::IsDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    LOG_INFO("Found GPU : %s", deviceProperties.deviceName);
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAcceptable = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
        swapChainAcceptable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
            && indices.isComplete()
            && extensionsSupported
            && swapChainAcceptable) {
        LOG_INFO("Picked as main GPU : %s", deviceProperties.deviceName);
        m_gpuName = deviceProperties.deviceName;
        return true;
    } else
        return false;
}

bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Renderer::CheckValidationLayerSupport() const
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : m_validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
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

            for (auto& requiredLayer : m_validationLayers)
            {
                LOG_ERROR(requiredLayer);
            }

            return false;
        }
    }

    return true;
}

std::vector<const char*> Renderer::GetRequiredExtensions()
{    
    std::vector<const char*> m_extensions;
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

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
    if (!s_enableValidationLayers) return true;

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
