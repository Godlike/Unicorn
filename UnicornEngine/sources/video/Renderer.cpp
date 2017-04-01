/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/Renderer.hpp>
#include <unicorn/core/Settings.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

#include <cstring>
#include <iostream>
#include <set>
#include <algorithm>
#include <array>
#include <tuple>

namespace unicorn
{
namespace video
{
static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        LOG_ERROR("VULKAN LAYER ERROR: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        LOG_WARNING("VULKAN LAYER WARNING: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER PERFORMANCE: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER INFO: %s", msg);
    }
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
        LOG_INFO("VULKAN LAYER DEBUG: %s", msg);
    }
    return VK_FALSE;
}

Renderer::Renderer()
    : m_isInitialized(false)
    , m_pWindow(nullptr)
    , m_validationLayers({"VK_LAYER_LUNARG_standard_validation"})
    , m_deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME})
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if (!glfwVulkanSupported())
    {
        LOG_ERROR("Vulkan not supported!");

        return false;
    }

    const core::Settings& settings = core::Settings::Instance();

    m_pWindow = glfwCreateWindow(settings.GetApplicationWidth(),
        settings.GetApplicationHeight(),
        settings.GetApplicationName().c_str(),
        nullptr,
        nullptr);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowSizeCallback(m_pWindow, OnWindowResized);
    if (!CreateInstance() ||
        !SetupDebugCallback() ||
        !CreateSurface() ||
        !PickPhysicalDevice() ||
        !CreateLogicalDevice() ||
        !CreateSwapChain() ||
        !CreateImageViews() ||
        !CreateRenderPass() ||
        !CreateGraphicsPipeline() ||
        !CreateFramebuffers() ||
        !CreateCommandPool() ||
        !CreateCommandBuffers() ||
        !CreateSemaphores())
    {
        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Vulkan render initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    FreeSemaphores();
    FreeCommandBuffers();
    FreeCommandPool();
    FreeFrameBuffers();
    FreeGraphicsPipeline();
    FreeRenderPass();
    FreeImageViews();
    FreeSwapChain();
    FreeLogicalDevice();
    FreeSurface();
    FreeDebugCallback();
    FreeInstance();

    if (m_pWindow)
    {
        glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
        glfwDestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    if (m_isInitialized)
    {
        LOG_INFO("Vulkan render shutdown correctly.");
    }

    m_isInitialized = false;
}

QueueFamilyIndices Renderer::FindQueueFamilies(const vk::PhysicalDevice& device)
{
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int index = 0;
    vk::Bool32 presentSupport;
    vk::Result result;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = index;
        }

        std::tie(result, presentSupport) = device.getSurfaceSupportKHR(index, m_vkWindowSurface);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = index++;
        }

        if (indices.IsComplete())
        {
            break;
        }
    }

    return indices;
}

bool Renderer::QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device)
{
    vk::Result result;
    result = device.getSurfaceCapabilitiesKHR(m_vkWindowSurface, &details.capabilities);
    std::tie(result, details.formats) = device.getSurfaceFormatsKHR(m_vkWindowSurface);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't get surface formats khr.");
        return false;
    }
    std::tie(result, details.presentModes) = device.getSurfacePresentModesKHR(m_vkWindowSurface);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't get surface present modes khr.");
        return false;
    }
    return true;
}

vk::SurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
    {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR Renderer::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Renderer::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    const core::Settings& settings = core::Settings::Instance();

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D actualExtent = {settings.GetApplicationWidth(), settings.GetApplicationHeight()};

        actualExtent.width = std::max(capabilities.minImageExtent.width,
            std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
            std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

void Renderer::Render()
{
    if (m_isInitialized && m_pWindow)
    {
        while (!glfwWindowShouldClose(m_pWindow))
        {
            glfwPollEvents();
            if (!Frame())
            {
                break;
            }
        }
        m_vkLogicalDevice.waitIdle();
    }
}

void Renderer::OnWindowResized(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    Renderer* renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (!renderer->RecreateSwapChain())
    {
        LOG_ERROR("Can't recreate swapchain!");
    }
}

bool Renderer::RecreateSwapChain()
{
    m_vkLogicalDevice.waitIdle();

    if (CreateSwapChain() &&
        CreateImageViews() &&
        CreateRenderPass() &&
        CreateGraphicsPipeline() &&
        CreateFramebuffers() &&
        CreateCommandBuffers())
    {
        return true;
    }
    return false;
}

void Renderer::FreeInstance()
{
    if (m_vkInstance)
    {
        m_vkInstance.destroy();
        m_vkInstance = nullptr;
    }
}

void Renderer::FreeDebugCallback()
{
    if (m_vulkanCallback != VK_NULL_HANDLE && m_vkLogicalDevice)
    {
        DestroyDebugReportCallbackEXT();
    }
}

void Renderer::FreeSurface()
{
    if (m_vkWindowSurface && m_vkLogicalDevice)
    {
        m_vkInstance.destroySurfaceKHR(m_vkWindowSurface);
        m_vkWindowSurface = nullptr;
    }
}

void Renderer::FreeLogicalDevice()
{
    if (m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroy();
        m_vkLogicalDevice = nullptr;
    }
}

void Renderer::FreeSwapChain()
{
    if (m_vkSwapChain && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroySwapchainKHR(m_vkSwapChain);
        m_vkSwapChain = nullptr;
    }
}

void Renderer::FreeImageViews()
{
    if (m_vkLogicalDevice)
    {
        for (vk::ImageView& view : m_swapChainImageViews)
        {
            m_vkLogicalDevice.destroyImageView(view);
        }
        m_swapChainImageViews.clear();
    }
}

void Renderer::FreeRenderPass()
{
    if (m_renderPass && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroyRenderPass(m_renderPass);
        m_renderPass = nullptr;
    }
}

void Renderer::FreeGraphicsPipeline()
{
    if (m_pipelineLayout && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroyPipelineLayout(m_pipelineLayout);
        m_pipelineLayout = nullptr;
    }

    if (m_graphicsPipeline && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroyPipeline(m_graphicsPipeline);
        m_graphicsPipeline = nullptr;
    }
}

void Renderer::FreeFrameBuffers()
{
    if (m_vkLogicalDevice)
    {
        for (vk::Framebuffer& framebuffer : m_swapChainFramebuffers)
        {
            m_vkLogicalDevice.destroyFramebuffer(framebuffer);
        }
        m_swapChainFramebuffers.clear();
    }
}

void Renderer::FreeCommandPool()
{
    if (m_commandPool && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroyCommandPool(m_commandPool);
        m_commandPool = nullptr;
    }
}

void Renderer::FreeCommandBuffers()
{
    if (!m_commandBuffers.empty() && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());
    }
}

void Renderer::FreeSemaphores()
{
    if (m_imageAvailableSemaphore && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroySemaphore(m_imageAvailableSemaphore);
        m_imageAvailableSemaphore = nullptr;
    }

    if (m_renderFinishedSemaphore && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroySemaphore(m_renderFinishedSemaphore);
        m_renderFinishedSemaphore = nullptr;
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

    vk::ApplicationInfo appInfo(settings.GetApplicationName().c_str(),
        VK_MAKE_VERSION(1, 0, 0),
        settings.GetUnicornEngineName().c_str(),
        VK_MAKE_VERSION(0, 1, 0),
        VK_API_VERSION_1_0);
    vk::InstanceCreateInfo createInfo = {};
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

    if (vk::createInstance(&createInfo, nullptr, &m_vkInstance) != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create instance!");
        return false;
    }

    return true;
}

bool Renderer::PickPhysicalDevice()
{
    vk::Result result;
    std::vector<vk::PhysicalDevice> devices;
    std::tie(result, devices) = m_vkInstance.enumeratePhysicalDevices();
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to enumerate physical devices.");
        return false;
    }
    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device))
        {
            m_vkPhysicalDevice = device;
            break;
        }
    }

    if (!m_vkPhysicalDevice)
    {
        LOG_ERROR("Failed to find a suitable GPU!");
        return false;
    }

    return true;
}

bool Renderer::CreateLogicalDevice()
{
    vk::Result result;
    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures; // No features for now.

    vk::DeviceCreateInfo createInfo;
    createInfo.setPQueueCreateInfos(queueCreateInfos.data());
    createInfo.setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()));
    createInfo.setPEnabledFeatures(&deviceFeatures);
    createInfo.setEnabledExtensionCount(static_cast<uint32_t>(m_deviceExtensions.size()));
    createInfo.setPpEnabledExtensionNames(m_deviceExtensions.data());

    if (s_enableValidationLayers)
    {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(m_validationLayers.size()));
        createInfo.setPpEnabledLayerNames(m_validationLayers.data());
    }
    else
    {
        createInfo.setEnabledLayerCount(0);
    }

    result = m_vkPhysicalDevice.createDevice(&createInfo, {}, &m_vkLogicalDevice);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't initialize Vulkan logical device!");
        return false;
    }
    m_graphicsQueue = m_vkLogicalDevice.getQueue(indices.graphicsFamily, 0);
    m_presentQueue = m_vkLogicalDevice.getQueue(indices.presentFamily, 0);

    return true;
}

bool Renderer::CreateSurface()
{
    if (glfwCreateWindowSurface(m_vkInstance, m_pWindow, nullptr, reinterpret_cast<VkSurfaceKHR*>(&m_vkWindowSurface)) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create window surface!");
        return false;
    }

    return true;
}

bool Renderer::CreateSwapChain()
{
    vk::Result result;
    SwapChainSupportDetails swapChainSupport;
    if (!QuerySwapChainSupport(swapChainSupport, m_vkPhysicalDevice))
    {
        return false;
    }
    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = m_vkWindowSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment; // VK_IMAGE_USAGE_TRANSFER_DST_BIT for post processing.

    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);
    uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    vk::SwapchainKHR oldSwapChain = m_vkSwapChain;
    if (oldSwapChain)
    {
        createInfo.oldSwapchain = oldSwapChain;
    }

    vk::SwapchainKHR newSwapChain;
    result = m_vkLogicalDevice.createSwapchainKHR(&createInfo, {}, &newSwapChain);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create Vulkan swap chain!");
    }

    m_vkSwapChain = newSwapChain;

    if (oldSwapChain)
    {
        m_vkLogicalDevice.destroySwapchainKHR(oldSwapChain);
        oldSwapChain = nullptr;
    }
    std::tie(result, m_swapChainImages) = m_vkLogicalDevice.getSwapchainImagesKHR(m_vkSwapChain);
    if (imageCount != m_swapChainImages.size() || result != vk::Result::eSuccess)
    {
        LOG_ERROR("SwapChain images not equal!");
    }
    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    return true;
}

bool Renderer::CreateImageViews()
{
    vk::Result result;
    FreeImageViews();

    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (uint32_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        vk::ImageViewCreateInfo createInfo;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = vk::ImageViewType::e2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = vk::ComponentSwizzle::eIdentity;
        createInfo.components.g = vk::ComponentSwizzle::eIdentity;
        createInfo.components.b = vk::ComponentSwizzle::eIdentity;
        createInfo.components.a = vk::ComponentSwizzle::eIdentity;
        createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        result = m_vkLogicalDevice.createImageView(&createInfo, {}, &m_swapChainImageViews[i]);
        if (result != vk::Result::eSuccess)
        {
            LOG_ERROR("Failed to create image views!");
            return false;
        }
    }

    return true;
}

bool Renderer::CreateRenderPass()
{
    vk::Result result;
    FreeRenderPass();

    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    result = m_vkLogicalDevice.createRenderPass(&renderPassInfo, {}, &m_renderPass);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create render pass!");
        return false;
    }

    return true;
}

bool Renderer::CreateGraphicsPipeline()
{
    vk::Result result;
    FreeGraphicsPipeline();

    unicorn::utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();
    unicorn::utility::asset::Handler simpleVertShaderHandler = storage.Get("data/shaders/shader.vert.spv");
    unicorn::utility::asset::Handler simpleFragShaderHandler = storage.Get("data/shaders/shader.frag.spv");

    if (!simpleVertShaderHandler.IsValid() || !simpleFragShaderHandler.IsValid())
    {
        LOG_ERROR("Can't find shaders!");
        return false;
    }

    vk::ShaderModule vertShaderModule, fragShaderModule;
    bool shadersCreatedFailed = !CreateShaderModule(simpleVertShaderHandler.GetContent().GetBuffer(), vertShaderModule) || !CreateShaderModule(simpleFragShaderHandler.GetContent().GetBuffer(), fragShaderModule);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

    vk::Viewport viewport;
    viewport.width = static_cast<float>(m_swapChainExtent.width);
    viewport.height = static_cast<float>(m_swapChainExtent.height);
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor;
    scissor.offset.setX(0);
    scissor.offset.setY(0);
    scissor.extent = m_swapChainExtent;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;

    vk::PipelineMultisampleStateCreateInfo multisampling; // TODO: configure MSAA at global level.
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = 0;

    result = m_vkLogicalDevice.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_pipelineLayout);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create pipeline layout!");
        return false;
    }

    if (shadersCreatedFailed)
    {
        LOG_ERROR("Can't create shader module!");
        return false;
    }

    vk::GraphicsPipelineCreateInfo pipelineInfo;
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
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1; // Optional

    std::tie(result, m_graphicsPipeline) = m_vkLogicalDevice.createGraphicsPipeline({}, pipelineInfo);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create graphics pipeline.");
        return false;
    }
    m_vkLogicalDevice.destroyShaderModule(vertShaderModule);
    m_vkLogicalDevice.destroyShaderModule(fragShaderModule);

    return true;
}

bool Renderer::CreateFramebuffers()
{
    vk::Result result;
    FreeFrameBuffers();

    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
    {
        vk::ImageView attachments[] = {m_swapChainImageViews[i]};

        vk::FramebufferCreateInfo framebufferInfo;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        result = m_vkLogicalDevice.createFramebuffer(&framebufferInfo, nullptr, &m_swapChainFramebuffers[i]);

        if (result != vk::Result::eSuccess)
        {
            LOG_ERROR("Failed to create framebuffer!");
            return false;
        }
    }

    return true;
}

bool Renderer::CreateCommandPool()
{
    vk::Result result;
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_vkPhysicalDevice);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    result = m_vkLogicalDevice.createCommandPool(&poolInfo, {}, &m_commandPool);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create command pool!");
        return false;
    }

    return true;
}

bool Renderer::CreateCommandBuffers()
{
    vk::Result result;
    FreeCommandBuffers();

    m_commandBuffers.resize(m_swapChainFramebuffers.size());

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    result = m_vkLogicalDevice.allocateCommandBuffers(&allocInfo, m_commandBuffers.data());

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to allocate command buffers!");
        return false;
    }

    for (size_t i = 0; i < m_commandBuffers.size(); ++i)
    {
        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        m_commandBuffers[i].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassInfo;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
        renderPassInfo.renderArea.setOffset({0, 0});
        renderPassInfo.renderArea.extent = m_swapChainExtent;

        vk::ClearColorValue clearColor(std::array<float, 4>({{0.0f, 0.0f, 0.0f, 1.0f}}));
        vk::ClearValue clearValue(clearColor);

        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        m_commandBuffers[i].beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);
        m_commandBuffers[i].draw(3, 1, 0, 0);
        m_commandBuffers[i].endRenderPass();

        m_commandBuffers[i].end();
    }

    return true;
}

bool Renderer::CreateSemaphores()
{
    vk::SemaphoreCreateInfo semaphoreInfo;
    if (m_vkLogicalDevice.createSemaphore(&semaphoreInfo, {}, &m_imageAvailableSemaphore) == vk::Result::eSuccess &&
        m_vkLogicalDevice.createSemaphore(&semaphoreInfo, {}, &m_renderFinishedSemaphore) == vk::Result::eSuccess)
    {
        return true;
    }

    LOG_ERROR("Failed to create semaphores!");
    return false;
}

bool Renderer::CreateShaderModule(const std::vector<uint8_t>& code, vk::ShaderModule& shaderModule)
{
    vk::Result result;
    vk::ShaderModuleCreateInfo createInfo;
    if (code.size() % sizeof(uint32_t) != 0)
    {
        LOG_ERROR("Shader code size is not multiple of sizeof(uint32_t), look at VkShaderModuleCreateInfo(3) Manual Page.");
        return false;
    }
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    result = m_vkLogicalDevice.createShaderModule(&createInfo, {}, &shaderModule);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create shader module!");
        return false;
    }

    return true;
}

bool Renderer::IsDeviceSuitable(const vk::PhysicalDevice& device)
{
    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

    LOG_INFO("Found GPU : %s", deviceProperties.deviceName);
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAcceptable = false;

    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport;
        if (!QuerySwapChainSupport(swapChainSupport, device))
        {
            return false;
        }
        swapChainAcceptable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && indices.IsComplete() && extensionsSupported && swapChainAcceptable)
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

bool Renderer::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device)
{
    vk::Result result;
    std::vector<vk::ExtensionProperties> availableExtensions;
    std::tie(result, availableExtensions) = device.enumerateDeviceExtensionProperties();
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't enumerate device extension properties.");
        return false;
    }
    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Renderer::Frame()
{
    uint32_t imageIndex;
    vk::Result result = m_vkLogicalDevice.acquireNextImageKHR(
        m_vkSwapChain,
        std::numeric_limits<uint64_t>::max(),
        m_imageAvailableSemaphore,
        nullptr,
        &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        if (!RecreateSwapChain())
        {
            LOG_ERROR("Can't recreate swapchain!");
            return false;
        }
        return true;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }

    vk::SubmitInfo submitInfo;

    vk::Semaphore waitSemaphores[] = {m_imageAvailableSemaphore};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

    vk::Semaphore signalSemaphores[] = {m_renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = m_graphicsQueue.submit(1, &submitInfo, nullptr);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("failed to submit draw command buffer!");
        return false;
    }

    vk::PresentInfoKHR presentInfo;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {m_vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;
    result = m_presentQueue.presentKHR(&presentInfo);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        RecreateSwapChain();
        return true;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }

    return true;
}

bool Renderer::CheckValidationLayerSupport() const
{
    vk::Result result;
    std::vector<vk::LayerProperties> availableLayers;
    std::tie(result, availableLayers) = vk::enumerateInstanceLayerProperties();
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't enumerate instance layer properties!");
        return false;
    }
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
                LOG_ERROR("%s", requiredLayer);
            }

            return false;
        }
    }

    LOG_INFO("Picked next Vulkan layers :");

    for (auto& layer : m_validationLayers)
    {
        LOG_INFO("%s", layer);
    }

    return true;
}

std::vector<const char*> Renderer::GetRequiredExtensions()
{
    std::vector<const char*> m_extensions;
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; ++i)
    {
        m_extensions.push_back(glfwExtensions[i]);
    }

    if (s_enableValidationLayers)
    {
        m_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return m_extensions;
}

VkResult Renderer::CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo)
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

bool Renderer::SetupDebugCallback()
{
    if (!s_enableValidationLayers)
    {
        return true;
    }

    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = DebugCallback;

    return CreateDebugReportCallbackEXT(&createInfo) == VK_SUCCESS;
}
}
}
