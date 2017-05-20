/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Renderer.hpp>
#include <unicorn/Settings.hpp>
#include <unicorn/utility/Logger.hpp>
#include <unicorn/video/vulkan/ShaderProgram.hpp>
#include <unicorn/system/Manager.hpp>
#include <unicorn/system/Window.hpp>
#include <unicorn/video/vulkan/Context.hpp>
#include <unicorn/video/vulkan/VkMesh.hpp>
#include <unicorn/video/Camera.hpp>
#include <unicorn/utility/Memory.hpp>

#include <set>
#include <algorithm>
#include <tuple>
#include <chrono>

namespace unicorn
{
namespace video
{
namespace vulkan
{
bool QueueFamilyIndices::IsComplete() const
{
    return graphicsFamily >= 0 && presentFamily >= 0;
}

Renderer::Renderer(system::Manager& manager, system::Window* window)
    : video::Renderer(manager, window)
{
    m_pWindow->Destroyed.connect(this, &Renderer::OnWindowDestroyed);
}

Renderer::~Renderer()
{
    if (m_pWindow)
    {
        m_pWindow->Destroyed.disconnect(this, &Renderer::OnWindowDestroyed);
        m_pWindow->SizeChanged.disconnect(this, &Renderer::OnWindowSizeChanged);
    }

    for (const auto& mesh : m_vkMeshes)
    {
        mesh->ReallocatedOnGpu.disconnect(this, &vulkan::Renderer::OnMeshReallocated);
    }

    Renderer::Deinit();
}

bool Renderer::Init()
{
    if (m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Renderer initialization started.");

    if (!CreateSurface() ||
        !PickPhysicalDevice() ||
        !CreateLogicalDevice() ||
        !CreateSwapChain() ||
        !CreateImageViews() ||
        !CreateRenderPass() ||
        !PrepareUniformBuffers() ||
        !CreateDescriptionSetLayout() ||
        !CreateGraphicsPipeline() ||
        !CreateFramebuffers() ||
        !CreateCommandPool() ||
        !CreateSemaphores() ||
        !CreateCommandBuffers())
    {
        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Renderer initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    for (auto& vkmesh : m_vkMeshes)
    {
        vkmesh->DeallocateOnGPU();
    }

    FreeSemaphores();
    FreeCommandBuffers();
    FreeCommandPool();
    FreeFrameBuffers();
    FreeGraphicsPipeline();
    FreeDescriptorPoolAndLayouts();
    FreeUniforms();
    FreeRenderPass();
    FreeImageViews();
    FreeSwapChain();
    FreeSurface();
    FreeLogicalDevice();

    if (m_isInitialized)
    {
        LOG_INFO("Render shutdown correctly.");
    }

    m_isInitialized = false;
}

QueueFamilyIndices Renderer::FindQueueFamilies(const vk::PhysicalDevice& device) const
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

bool Renderer::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, vk::Format& returnFormat) const
{
    for (auto format : candidates)
    {
        vk::FormatProperties props;
        m_vkPhysicalDevice.getFormatProperties(format, &props);
        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            returnFormat = format;
            return true;
        }
        if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            returnFormat = format;
            return true;
        }
    }
    return false;
}

bool Renderer::FindDepthFormat(vk::Format& desiredFormat)
{
    return FindSupportedFormat({vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                               vk::ImageTiling::eOptimal,
                               vk::FormatFeatureFlagBits::eDepthStencilAttachment,
                               desiredFormat);
}

bool Renderer::HasStencilComponent(vk::Format format) const
{
    return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

bool Renderer::QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device)
{
    vk::Result result = device.getSurfaceCapabilitiesKHR(m_vkWindowSurface, &details.capabilities);
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
    const Settings& settings = Settings::Instance();

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    vk::Extent2D actualExtent = {settings.GetApplicationWidth(), settings.GetApplicationHeight()};

    actualExtent.width = std::max(capabilities.minImageExtent.width,
                                  std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height,
                                   std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
}

bool Renderer::Render()
{
    if (m_isInitialized && m_pWindow)
    {
        Frame();

        m_vkLogicalDevice.waitIdle();

        return true;
    }

    return false;
}

void Renderer::OnWindowDestroyed(system::Window* pWindow)
{
    LOG_INFO("Window destroyed, deinitializing renderer");

    m_pWindow = nullptr;

    Deinit();
}

void Renderer::OnWindowSizeChanged(system::Window* pWindow, std::pair<int32_t, int32_t> size)
{
    if (size.first == 0 || size.second == 0)
    {
        return;
    }

    if (!RecreateSwapChain())
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

void Renderer::OnMeshReallocated(VkMesh* vkmesh)
{
    m_uniformModel.Destroy();
    size_t bufferSize = m_vkMeshes.size() * m_dynamicAlignment;
    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible, bufferSize);
    if (m_uniformModelsData.model)
    {
        utility::AlignedFree(m_uniformModelsData.model);
    }
    m_uniformModelsData.model = static_cast<glm::mat4*>(utility::AlignedAlloc(bufferSize, m_dynamicAlignment));
    for (uint32_t i = 0; i < m_vkMeshes.size(); ++i)
    {
        glm::mat4* modelMat = reinterpret_cast<glm::mat4*>((reinterpret_cast<uint64_t>(m_uniformModelsData.model) + (i * m_dynamicAlignment)));
        *modelMat = m_vkMeshes.at(i)->GetModel();
    }
    m_uniformModel.Map();
    m_uniformModel.Write(m_uniformModelsData.model);
    ResizeDynamicUniformBuffer();
    CreateCommandBuffers();
}

std::shared_ptr<geometry::Mesh> Renderer::SpawnMesh()
{
    auto mesh = std::make_shared<geometry::Mesh>();
    auto vkmesh = new VkMesh(m_vkLogicalDevice, m_vkPhysicalDevice, m_commandPool, m_graphicsQueue, mesh);
    vkmesh->ReallocatedOnGpu.connect(this, &vulkan::Renderer::OnMeshReallocated);
    m_vkMeshes.push_back(vkmesh);
    m_meshes.push_back(mesh);
    return mesh;
}

void Renderer::FreeSurface()
{
    if (m_vkWindowSurface && m_vkLogicalDevice)
    {
        Context::GetVkInstance().destroySurfaceKHR(m_vkWindowSurface);
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
        m_vkLogicalDevice.freeCommandBuffers(m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
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

void Renderer::FreeUniforms()
{
    m_uniformMvp.Destroy();
    m_uniformModel.Destroy();
}

void Renderer::FreeDescriptorPoolAndLayouts()
{
    if (m_descriptorPool)
    {
        m_vkLogicalDevice.destroyDescriptorPool(m_descriptorPool);
    }
    if (m_descriptorSetLayout)
    {
        m_vkLogicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);
    }
    if (m_pipelineLayout)
    {
        m_vkLogicalDevice.destroyPipelineLayout(m_pipelineLayout);
    }
}

bool Renderer::PrepareUniformBuffers()
{
    size_t uboAlignment = m_physicalDeviceProperties.limits.minUniformBufferOffsetAlignment;
    m_dynamicAlignment = (sizeof(glm::mat4) / uboAlignment) * uboAlignment + ((sizeof(glm::mat4) % uboAlignment) > 0 ? uboAlignment : 0);

    m_uniformCameraData.proj = m_camera->GetProjection();
    m_uniformCameraData.view = m_camera->GetView();

    m_uniformMvp.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(UniformCameraData));
    m_uniformMvp.Map();
    m_uniformMvp.Write(&m_uniformCameraData);

    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible, m_dynamicAlignment);
    return true;
}

void Renderer::ResizeDynamicUniformBuffer()
{
    std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

    vk::WriteDescriptorSet writeDescriptorSetMVP;
    writeDescriptorSetMVP.dstSet = m_descriptorSet;
    writeDescriptorSetMVP.descriptorType = vk::DescriptorType::eUniformBuffer;
    writeDescriptorSetMVP.dstBinding = 0;
    writeDescriptorSetMVP.pBufferInfo = &m_uniformMvp.descriptor;
    writeDescriptorSetMVP.descriptorCount = 1;

    vk::WriteDescriptorSet writeDescriptorSetMODEL;
    writeDescriptorSetMODEL.dstSet = m_descriptorSet;
    writeDescriptorSetMODEL.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    writeDescriptorSetMODEL.dstBinding = 1;
    writeDescriptorSetMODEL.pBufferInfo = &m_uniformModel.descriptor;
    writeDescriptorSetMODEL.descriptorCount = 1;

    writeDescriptorSets.push_back(writeDescriptorSetMVP);
    writeDescriptorSets.push_back(writeDescriptorSetMODEL);

    m_vkLogicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

void Renderer::UpdateUniformBuffer()
{
    m_uniformCameraData.proj = m_camera->GetProjection();
    m_uniformCameraData.view = m_camera->GetView();
    m_uniformMvp.Write(&m_uniformCameraData);
}

void Renderer::UpdateDynamicUniformBuffer()
{
    for (uint32_t i = 0; i < m_vkMeshes.size(); ++i)
    {
        glm::mat4* modelMat = reinterpret_cast<glm::mat4*>((reinterpret_cast<uint64_t>(m_uniformModelsData.model) + (i * m_dynamicAlignment)));
        *modelMat = m_vkMeshes.at(i)->GetModel();
    }
    m_uniformModel.Write(m_uniformModelsData.model);

    vk::MappedMemoryRange mappedMemoryRange;
    mappedMemoryRange.memory = m_uniformModel.GetMemory();
    mappedMemoryRange.size = m_uniformModel.GetSize();
    m_vkLogicalDevice.flushMappedMemoryRanges(1, &mappedMemoryRange);
}

bool Renderer::PickPhysicalDevice()
{
    vk::Result result;
    std::vector<vk::PhysicalDevice> devices;
    std::tie(result, devices) = Context::GetVkInstance().enumeratePhysicalDevices();
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
    m_physicalDeviceProperties = m_vkPhysicalDevice.getProperties();
    return true;
}

bool Renderer::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures;

    vk::DeviceCreateInfo createInfo;
    createInfo.setPQueueCreateInfos(queueCreateInfos.data());
    createInfo.setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()));
    createInfo.setPEnabledFeatures(&deviceFeatures);
    createInfo.setEnabledExtensionCount(static_cast<uint32_t>(Context::deviceExtensions.size()));
    createInfo.setPpEnabledExtensionNames(Context::deviceExtensions.data());

    if (s_enableValidationLayers)
    {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(Context::validationLayers.size()));
        createInfo.setPpEnabledLayerNames(Context::validationLayers.data());
    }
    else
    {
        createInfo.setEnabledLayerCount(0);
    }

    vk::Result result = m_vkPhysicalDevice.createDevice(&createInfo, {}, &m_vkLogicalDevice);

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
    if (!m_pWindow || m_systemManager.CreateVulkanSurfaceForWindow(*m_pWindow, Context::GetVkInstance(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&m_vkWindowSurface)) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create window surface!");

        return false;
    }

    return true;
}

bool Renderer::CreateSwapChain()
{
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
    uint32_t queueFamilyIndices[] = {(uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.queueFamilyIndexCount = 0; // Optional
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
    vk::Result result = m_vkLogicalDevice.createSwapchainKHR(&createInfo, {}, &newSwapChain);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create Vulkan swap chain!");
        return false;
    }

    m_vkSwapChain = newSwapChain;

    if (oldSwapChain)
    {
        m_vkLogicalDevice.destroySwapchainKHR(oldSwapChain);
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
        vk::Result result = m_vkLogicalDevice.createImageView(&createInfo, {}, &m_swapChainImageViews[i]);
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

    vk::Result result = m_vkLogicalDevice.createRenderPass(&renderPassInfo, {}, &m_renderPass);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create render pass!");
        return false;
    }

    return true;
}

bool Renderer::CreateDescriptionSetLayout()
{
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
    vk::DescriptorPoolSize descriptorMVPPoolSize;
    descriptorMVPPoolSize.type = vk::DescriptorType::eUniformBuffer;
    descriptorMVPPoolSize.descriptorCount = 1;

    vk::DescriptorPoolSize descriptorModelPoolSize;
    descriptorModelPoolSize.type = vk::DescriptorType::eUniformBufferDynamic;
    descriptorModelPoolSize.descriptorCount = 1;

    descriptorPoolSizes.push_back(descriptorMVPPoolSize);
    descriptorPoolSizes.push_back(descriptorModelPoolSize);

    vk::DescriptorPoolCreateInfo poolCreateInfo;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    poolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
    poolCreateInfo.maxSets = 2;

    vk::Result result = m_vkLogicalDevice.createDescriptorPool(&poolCreateInfo, nullptr, &m_descriptorPool);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create descriptor pool!");
        return false;
    }

    std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings;

    vk::DescriptorSetLayoutBinding setMVP;
    setMVP.descriptorType = vk::DescriptorType::eUniformBuffer;
    setMVP.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setMVP.binding = 0; //TODO: hardcode description binding
    setMVP.descriptorCount = 1;

    vk::DescriptorSetLayoutBinding setMODEL;
    setMODEL.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    setMODEL.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setMODEL.binding = 1; //TODO: hardcode description binding
    setMODEL.descriptorCount = 1;

    descriptorSetLayoutBindings.push_back(setMVP);
    descriptorSetLayoutBindings.push_back(setMODEL);

    vk::DescriptorSetLayoutCreateInfo descriptorLayout;
    descriptorLayout.pBindings = descriptorSetLayoutBindings.data();
    descriptorLayout.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBindings.size());

    result = m_vkLogicalDevice.createDescriptorSetLayout(&descriptorLayout, nullptr, &m_descriptorSetLayout);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create descriptor set layout!");
        return false;
    }

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;

    result = m_vkLogicalDevice.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_pipelineLayout);
    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create pipeline layout!");
        return false;
    }

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.pSetLayouts = &m_descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &m_descriptorSet);

    if (result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't allocate descriptor sets!");
        return false;
    }

    ResizeDynamicUniformBuffer();

    return true;
}

bool Renderer::CreateGraphicsPipeline()
{
    vk::Result result;
    FreeGraphicsPipeline();

    m_shaderProgram = new ShaderProgram(m_vkLogicalDevice, "data/shaders/shader.vert.spv", "data/shaders/shader.frag.spv");

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
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eNone;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.polygonMode = vk::PolygonMode::eFill;

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

    if (!m_shaderProgram->IsCreated())
    {
        LOG_ERROR("Can't create shader module!");
        return false;
    }

    auto vertexInputInfo = m_shaderProgram->GetVertexInputInfo();

    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = m_shaderProgram->GetShaderStageInfoData();
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
    m_shaderProgram->DestroyShaderModules();

    return true;
}

bool Renderer::CreateFramebuffers()
{
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

        vk::Result result = m_vkLogicalDevice.createFramebuffer(&framebufferInfo, nullptr, &m_swapChainFramebuffers[i]);

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
    QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_vkPhysicalDevice);

    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    vk::Result result = m_vkLogicalDevice.createCommandPool(&poolInfo, {}, &m_commandPool);
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

        vk::ClearColorValue clearColor(m_backgroundColor);
        vk::ClearValue clearValue(clearColor);

        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        m_commandBuffers[i].beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);

        vk::DeviceSize offsets[] = {0};
        for (uint32_t j = 0; j < m_vkMeshes.size(); ++j)
        {
            vk::Buffer vertexBuffer[] = {m_vkMeshes.at(j)->GetVertexBuffer()};
            uint32_t dynamicOffset = j * static_cast<uint32_t>(m_dynamicAlignment);
            m_commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffer, offsets);
            m_commandBuffers[i].bindIndexBuffer(m_vkMeshes.at(j)->GetIndexBuffer(), 0, vk::IndexType::eUint16);
            m_commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1, &m_descriptorSet, 1, &dynamicOffset);
            m_commandBuffers[i].drawIndexed(m_vkMeshes.at(j)->IndicesSize(), 1, 0, 0, 0);
        }

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
    return false;
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
    auto deviceExtensions = Context::deviceExtensions;
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool Renderer::Frame()
{
    uint32_t imageIndex;
    vk::Result result = m_vkLogicalDevice.acquireNextImageKHR(m_vkSwapChain,
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
    if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
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

    UpdateUniformBuffer();
    UpdateDynamicUniformBuffer();

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
    if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }

    return true;
}
}
}
}
