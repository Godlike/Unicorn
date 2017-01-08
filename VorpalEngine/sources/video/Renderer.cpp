#include <vorpal/video/Renderer.hpp>
#include <vorpal/core/Settings.hpp>
#include <vorpal/utility/Logger.hpp>
#include <cstring>
#include <iostream>

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
    if (!CreateInstance())    
        return false;   
    if(SetupDebugCallback() != VK_SUCCESS)
        LOG_ERROR("Can't setup debug callback'");
    if(!PickPhysicalDevice())
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

QueueFamilyIndices Renderer::findQueueFamilies()
{
    QueueFamilyIndices indices;

    return indices;
}

VkResult Renderer::CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(m_vkInstance, pCreateInfo, nullptr, &m_vulkanCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Renderer::DestroyDebugReportCallbackEXT() {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT");
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

bool Renderer::IsDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    LOG_INFO("Found GPU : %s", deviceProperties.deviceName);
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ;
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
    return CreateDebugReportCallbackEXT(&createInfo);
}

}
}
