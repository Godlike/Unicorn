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
#include <unicorn/video/vulkan/VkTexture.hpp>
#include <unicorn/video/Camera.hpp>
#include <unicorn/utility/Memory.hpp>
#include <unicorn/video/Texture.hpp>
#include <unicorn/video/Material.hpp>

#include <glm/gtc/type_ptr.hpp>

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
const uint32_t Renderer::s_swapChainAttachmentsAmount = 2;

#ifdef NDEBUG
const bool Renderer::s_enableValidationLayers = false;
#else
const bool Renderer::s_enableValidationLayers = true;
#endif

bool QueueFamilyIndices::IsComplete() const
{
    return graphicsFamily >= 0 && presentFamily >= 0;
}

Renderer::Renderer(system::Manager& manager, system::Window* window)
    : video::Renderer(manager, window)
    , m_depthImage(nullptr)
    , m_hasDirtyMeshes(false)
{
    m_pWindow->Destroyed.connect(this, &Renderer::OnWindowDestroyed);
}

Renderer::~Renderer()
{
    Destroyed.emit(this);
    Destroyed.clear();

    if(m_pWindow)
    {
        m_pWindow->Destroyed.disconnect(this, &Renderer::OnWindowDestroyed);
        m_pWindow->SizeChanged.disconnect(this, &Renderer::OnWindowSizeChanged);
    }

    Deinit();
}

bool Renderer::Init()
{
    if(m_isInitialized)
    {
        return false;
    }

    LOG_INFO("Renderer initialization started.");

    if(!CreateSurface() ||
        !PickPhysicalDevice() ||
        !CreateLogicalDevice() ||
        !CreateSwapChain() ||
        !CreateImageViews() ||
        !FindDepthFormat(m_depthImageFormat) ||
        !CreateDepthBuffer() ||
        !CreateRenderPass() ||
        !PrepareUniformBuffers() ||
        !CreateDescriptionSetLayout() ||
        !CreateGraphicsPipeline() ||
        !CreateFramebuffers() ||
        !CreateCommandPool() ||
        !CreateSemaphores() ||
        !CreateCommandBuffers() ||
        !LoadEngineHelpData())
    {
        return false;
    }

    m_isInitialized = true;

    LOG_INFO("Renderer initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    if(m_isInitialized)
    {
        {
            // Create a copy of mesh list to delete all meshes
            std::list<Mesh*> meshes(m_meshes);

            for(auto& pMesh : meshes)
            {
                DeleteMesh(pMesh);
            }

            LOG_DEBUG("Deleted %u meshes", static_cast<uint32_t>(meshes.size()));

            if(!m_vkMeshes.empty())
            {
                // Also free up all remaining vk meshes
                for(auto& pVkMesh : m_vkMeshes)
                {
                    DeleteVkMesh(pVkMesh);
                }

                LOG_DEBUG("Deleted %u stray vk meshes", static_cast<uint32_t>(m_vkMeshes.size()));

                m_vkMeshes.clear();
            }
        }

        FreeTextures();
        FreeSemaphores();
        FreeCommandBuffers();
        FreeCommandPool();
        FreeFrameBuffers();
        FreeGraphicsPipeline();
        FreeDescriptorPoolAndLayouts();
        FreeUniforms();
        FreeRenderPass();
        FreeDepthBuffer();
        FreeImageViews();
        FreeSwapChain();
        FreeSurface();
        FreeLogicalDevice();

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
    for(const auto& queueFamily : queueFamilies)
    {
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = index;
        }

        std::tie(result, presentSupport) = device.getSurfaceSupportKHR(index, m_vkWindowSurface);

        if(queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = index++;
        }

        if(indices.IsComplete())
        {
            break;
        }
    }

    return indices;
}

bool Renderer::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, vk::Format& returnFormat) const
{
    for(const auto& format : candidates)
    {
        vk::FormatProperties props;
        m_vkPhysicalDevice.getFormatProperties(format, &props);
        if(tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            returnFormat = format;
            return true;
        }
        if(tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            returnFormat = format;
            return true;
        }
    }
    return false;
}

bool Renderer::FindDepthFormat(vk::Format& desiredFormat) const
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

bool Renderer::QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device) const
{
    vk::Result result = device.getSurfaceCapabilitiesKHR(m_vkWindowSurface, &details.capabilities);
    std::tie(result, details.formats) = device.getSurfaceFormatsKHR(m_vkWindowSurface);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't get surface formats khr.");
        return false;
    }
    std::tie(result, details.presentModes) = device.getSurfacePresentModesKHR(m_vkWindowSurface);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't get surface present modes khr.");
        return false;
    }
    return true;
}

vk::SurfaceFormatKHR Renderer::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
{
    if(availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
    {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR Renderer::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const
{
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Renderer::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const
{
    const Settings& settings = Settings::Instance();

    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
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
    if(m_isInitialized && m_pWindow)
    {
        if(m_hasDirtyMeshes)
        {
            // Update all related data
            OnMeshReallocated(nullptr);
            CreateCommandBuffers();
            m_hasDirtyMeshes = false;
        }

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
    if(size.first == 0 || size.second == 0)
    {
        return;
    }

    if(!RecreateSwapChain())
    {
        LOG_ERROR("Can't recreate swapchain!");
    }
    if(!CreateDepthBuffer())
    {
        LOG_ERROR("Can't recreate depth buffer!");
    }
}

bool Renderer::RecreateSwapChain()
{
    m_vkLogicalDevice.waitIdle();

    if(CreateSwapChain() &&
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

bool Renderer::AddMesh(Mesh* mesh)
{
    auto vkmesh = new VkMesh(m_vkLogicalDevice, m_vkPhysicalDevice, m_commandPool, m_graphicsQueue, *mesh);

    if(!AllocateMaterial(*mesh, *vkmesh))
    {
        LOG_ERROR("Can't allocate material!");
        return false;
    }

    vkmesh->ReallocatedOnGpu.connect(this, &vulkan::Renderer::OnMeshReallocated);
    vkmesh->MaterialUpdated.connect(this, &vulkan::Renderer::OnMeshMaterialUpdated); // TODO: return error from here

    m_vkMeshes.push_back(vkmesh);
    m_meshes.push_back(mesh);
    vkmesh->AllocateOnGPU();

    CreateCommandBuffers();

    return true;
}

void Renderer::OnMeshReallocated(VkMesh* /*vkmesh*/)
{
    m_uniformModel.Destroy();
    size_t bufferSize = m_vkMeshes.size() * m_dynamicAlignment;
    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible, bufferSize);

    if(m_uniformModelsData.model)
    {
        utility::AlignedFree(m_uniformModelsData.model);
    }

    m_uniformModelsData.model = static_cast<glm::mat4*>(utility::AlignedAlloc(bufferSize, m_dynamicAlignment));

    UpdateVkMeshMatrices();

    m_uniformModel.Map();
    m_uniformModel.Write(m_uniformModelsData.model);

    UpdateMVPDescriptorSet();
}

void Renderer::OnMeshMaterialUpdated(Mesh* mesh, VkMesh* vkMesh)
{
    AllocateMaterial(*mesh, *vkMesh);
    CreateCommandBuffers();
}

bool Renderer::DeleteMesh(const Mesh* mesh)
{
    auto vkMeshIt = std::find_if(m_vkMeshes.begin(), m_vkMeshes.end(), [=](VkMesh* p) ->bool { return *p == *mesh; });

    if(vkMeshIt != m_vkMeshes.end())
    {
        DeleteVkMesh(*vkMeshIt);

        m_vkMeshes.erase(vkMeshIt);

        m_hasDirtyMeshes = true;
    }

    auto meshIt = std::find(m_meshes.begin(), m_meshes.end(), mesh);

    if(meshIt != m_meshes.end())
    {
        delete *meshIt;
        m_meshes.erase(meshIt);

        return true;
    }
    else
    {
        return false;
    }
}

void Renderer::SetDepthTest(bool enabled)
{
    m_depthTestEnabled = enabled;
    CreateGraphicsPipeline();
}

void Renderer::DeleteVkMesh(VkMesh* pVkMesh)
{
    pVkMesh->DeallocateOnGPU();
    delete pVkMesh;
}

void Renderer::FreeSurface()
{
    if(m_vkLogicalDevice)
    {
        if(m_vkWindowSurface)
        {
            Context::Instance().GetVkInstance().destroySurfaceKHR(m_vkWindowSurface);
            m_vkWindowSurface = nullptr;
        }
    }
}

void Renderer::FreeLogicalDevice()
{
    if(m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroy();
        m_vkLogicalDevice = nullptr;
    }
}

void Renderer::FreeSwapChain()
{
    if(m_vkLogicalDevice)
    {
        if(m_vkSwapChain)
        {
            m_vkLogicalDevice.destroySwapchainKHR(m_vkSwapChain);
            m_vkSwapChain = nullptr;
        }
    }
}

void Renderer::FreeImageViews()
{
    if(m_vkLogicalDevice)
    {
        for(vk::ImageView& view : m_swapChainImageViews)
        {
            m_vkLogicalDevice.destroyImageView(view);
        }
        m_swapChainImageViews.clear();
    }
}

void Renderer::FreeDepthBuffer()
{
    if(m_depthImage && m_depthImage->IsInitialized())
    {
        delete m_depthImage;
        m_depthImage = nullptr;
    }
}

void Renderer::FreeRenderPass()
{
    if(m_vkLogicalDevice)
    {
        if(m_renderPass)
        {
            m_vkLogicalDevice.destroyRenderPass(m_renderPass);
            m_renderPass = nullptr;
        }
    }
}

void Renderer::FreeGraphicsPipeline()
{
    if(m_vkLogicalDevice)
    {
        if(m_pipelines.solid)
        {
            m_vkLogicalDevice.destroyPipeline(m_pipelines.solid);
            m_pipelines.solid = nullptr;
        }
        if(m_pipelines.blend)
        {
            m_vkLogicalDevice.destroyPipeline(m_pipelines.blend);
            m_pipelines.blend = nullptr;
        }
        if(m_pipelines.wired)
        {
            m_vkLogicalDevice.destroyPipeline(m_pipelines.wired);
            m_pipelines.wired = nullptr;
        }
    }
}

void Renderer::FreeFrameBuffers()
{
    if(m_vkLogicalDevice)
    {
        for(vk::Framebuffer& framebuffer : m_swapChainFramebuffers)
        {
            m_vkLogicalDevice.destroyFramebuffer(framebuffer);
        }
        m_swapChainFramebuffers.clear();
    }
}

void Renderer::FreeCommandPool()
{
    if(m_commandPool && m_vkLogicalDevice)
    {
        m_vkLogicalDevice.destroyCommandPool(m_commandPool);
        m_commandPool = nullptr;
    }
}

void Renderer::FreeCommandBuffers()
{
    if(m_vkLogicalDevice)
    {
        if(!m_commandBuffers.empty())
        {
            m_vkLogicalDevice.freeCommandBuffers(m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
        }
    }
}

void Renderer::FreeSemaphores()
{
    if(m_vkLogicalDevice)
    {
        if(m_imageAvailableSemaphore)
        {
            m_vkLogicalDevice.destroySemaphore(m_imageAvailableSemaphore);
            m_imageAvailableSemaphore = nullptr;
        }

        if(m_renderFinishedSemaphore)
        {
            m_vkLogicalDevice.destroySemaphore(m_renderFinishedSemaphore);
            m_renderFinishedSemaphore = nullptr;
        }
    }
}

void Renderer::FreeUniforms()
{
    m_uniformViewProjection.Destroy();
    m_uniformModel.Destroy();
}

void Renderer::FreeDescriptorPoolAndLayouts() const
{
    if(m_vkLogicalDevice)
    {
        if(m_pipelineLayout)
        {
            m_vkLogicalDevice.destroyPipelineLayout(m_pipelineLayout);
        }
        for(auto& descriptorSetLayout : m_descriptorSetLayouts)
        {
            if(descriptorSetLayout)
            {
                m_vkLogicalDevice.destroyDescriptorSetLayout(descriptorSetLayout);
            }
        }
        if(m_descriptorPool)
        {
            m_vkLogicalDevice.destroyDescriptorPool(m_descriptorPool);
        }
    }
}

void Renderer::FreePipelineCache()
{
    if(m_vkLogicalDevice)
    {
        if(pipelineCache)
        {
            m_vkLogicalDevice.destroyPipelineCache(pipelineCache);
        }
    }
}

void Renderer::FreeTextures()
{
    for(auto& vkTexture : m_texturesCache)
    {
        vkTexture.second->Delete();
    }
    m_texturesCache.clear();
}

bool Renderer::PrepareUniformBuffers()
{
    size_t uboAlignment = static_cast<size_t>(m_physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
    m_dynamicAlignment = (sizeof( glm::mat4) / uboAlignment) * uboAlignment + ((sizeof( glm::mat4) % uboAlignment) > 0 ? uboAlignment : 0);

    m_uniformCameraData.proj = m_camera.GetProjection();
    m_uniformCameraData.view = m_camera.GetView();

    m_uniformViewProjection.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sizeof(UniformCameraData));
    m_uniformViewProjection.Map();
    m_uniformViewProjection.Write(&m_uniformCameraData);

    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible, m_dynamicAlignment);
    return true;
}

void Renderer::UpdateMVPDescriptorSet()
{
    std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

    // View projection uniform
    vk::WriteDescriptorSet viewProjectionWriteSet;
    viewProjectionWriteSet.dstSet = m_mvpDescriptorSet;
    viewProjectionWriteSet.descriptorType = vk::DescriptorType::eUniformBuffer;
    viewProjectionWriteSet.dstBinding = 0;
    viewProjectionWriteSet.pBufferInfo = &m_uniformViewProjection.GetDescriptorInfo();
    viewProjectionWriteSet.descriptorCount = 1;

    // Model uniform
    vk::WriteDescriptorSet modelWriteSet;
    modelWriteSet.dstSet = m_mvpDescriptorSet;
    modelWriteSet.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    modelWriteSet.dstBinding = 1;
    modelWriteSet.pBufferInfo = &m_uniformModel.GetDescriptorInfo();
    modelWriteSet.descriptorCount = 1;

    writeDescriptorSets.push_back(viewProjectionWriteSet);
    writeDescriptorSets.push_back(modelWriteSet);

    m_vkLogicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

void Renderer::UpdateUniformBuffer()
{
    if(m_uniformCameraData.view != m_camera.GetView() || m_uniformCameraData.proj != m_camera.GetProjection())
    {
        m_uniformCameraData.proj = m_camera.GetProjection();
        m_uniformCameraData.view = m_camera.GetView();
        m_uniformViewProjection.Write(&m_uniformCameraData);
    }
}

void Renderer::UpdateDynamicUniformBuffer()
{
    UpdateVkMeshMatrices();

    m_uniformModel.Write(m_uniformModelsData.model);

    vk::MappedMemoryRange mappedMemoryRange;
    mappedMemoryRange.memory = m_uniformModel.GetMemory();
    mappedMemoryRange.size = m_uniformModel.GetSize();
    m_vkLogicalDevice.flushMappedMemoryRanges(1, &mappedMemoryRange);
}

void Renderer::UpdateVkMeshMatrices()
{
    glm::mat4* pModelMat = nullptr;
    uint32_t i = 0;

    for(auto pVkMesh : m_vkMeshes)
    {
        // Get model matrix location
        pModelMat = reinterpret_cast<glm::mat4*>(
            (reinterpret_cast<uint64_t>(m_uniformModelsData.model) + (i * m_dynamicAlignment))
        );

        // Fill model matrix
        *pModelMat = pVkMesh->GetModelMatrix();

        ++i;
    }
}

bool Renderer::PickPhysicalDevice()
{
    vk::Result result;
    std::vector<vk::PhysicalDevice> devices;
    std::tie(result, devices) = Context::Instance().GetVkInstance().enumeratePhysicalDevices();
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to enumerate physical devices.");
        return false;
    }
    for(const auto& device : devices)
    {
        if(IsDeviceSuitable(device))
        {
            m_vkPhysicalDevice = device;
            break;
        }
    }

    if(!m_vkPhysicalDevice)
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

    for(int queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    m_deviceFeatures.setSamplerAnisotropy(VK_TRUE);
    m_deviceFeatures.setFillModeNonSolid(VK_TRUE);

    vk::DeviceCreateInfo createInfo;
    createInfo.setPQueueCreateInfos(queueCreateInfos.data());
    createInfo.setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()));
    createInfo.setPEnabledFeatures(&m_deviceFeatures);
    createInfo.setEnabledExtensionCount(static_cast<uint32_t>(Context::Instance().GetDeviceExtensions().size()));
    createInfo.setPpEnabledExtensionNames(Context::Instance().GetDeviceExtensions().data());

    if(s_enableValidationLayers)
    {
        createInfo.setEnabledLayerCount(static_cast<uint32_t>(Context::Instance().GetValidationLayers().size()));
        createInfo.setPpEnabledLayerNames(Context::Instance().GetValidationLayers().data());
    }
    else
    {
        createInfo.setEnabledLayerCount(0);
    }

    vk::Result result = m_vkPhysicalDevice.createDevice(&createInfo, {}, &m_vkLogicalDevice);

    if(result != vk::Result::eSuccess)
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
    if(!m_pWindow || m_systemManager.CreateVulkanSurfaceForWindow(*m_pWindow, Context::Instance().GetVkInstance(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&m_vkWindowSurface)) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create window surface!");

        return false;
    }

    return true;
}

bool Renderer::CreateSwapChain()
{
    SwapChainSupportDetails swapChainSupport;
    if(!QuerySwapChainSupport(swapChainSupport, m_vkPhysicalDevice))
    {
        return false;
    }
    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
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
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    QueueFamilyIndices indices = FindQueueFamilies(m_vkPhysicalDevice);
    uint32_t queueFamilyIndices[] = {static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentFamily)};

    if(indices.graphicsFamily != indices.presentFamily)
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
    if(oldSwapChain)
    {
        createInfo.oldSwapchain = oldSwapChain;
    }

    vk::SwapchainKHR newSwapChain;
    vk::Result result = m_vkLogicalDevice.createSwapchainKHR(&createInfo, {}, &newSwapChain);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create Vulkan swap chain!");
        return false;
    }

    m_vkSwapChain = newSwapChain;

    if(oldSwapChain)
    {
        m_vkLogicalDevice.destroySwapchainKHR(oldSwapChain);
    }
    std::tie(result, m_swapChainImages) = m_vkLogicalDevice.getSwapchainImagesKHR(m_vkSwapChain);
    if(imageCount != m_swapChainImages.size() || result != vk::Result::eSuccess)
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

    for(uint32_t i = 0; i < m_swapChainImages.size(); ++i)
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
        if(result != vk::Result::eSuccess)
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

    vk::AttachmentDescription attachments[s_swapChainAttachmentsAmount];

    attachments[0].format = m_swapChainImageFormat;
    attachments[0].samples = vk::SampleCountFlagBits::e1;
    attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
    attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
    attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachments[0].initialLayout = vk::ImageLayout::eUndefined;
    attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

    attachments[1].format = m_depthImageFormat;
    attachments[1].samples = vk::SampleCountFlagBits::e1;
    attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
    attachments[1].storeOp = vk::AttachmentStoreOp::eDontCare;
    attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachments[1].initialLayout = vk::ImageLayout::eUndefined;
    attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference attachmentRef[s_swapChainAttachmentsAmount];
    attachmentRef[0].attachment = 0;
    attachmentRef[0].layout = vk::ImageLayout::eColorAttachmentOptimal;

    attachmentRef[1].attachment = 1;
    attachmentRef[1].layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &attachmentRef[0];
    subpass.pDepthStencilAttachment = &attachmentRef[1];

    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = s_swapChainAttachmentsAmount;
    renderPassInfo.pAttachments = attachments;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vk::Result result = m_vkLogicalDevice.createRenderPass(&renderPassInfo, {}, &m_renderPass);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create render pass!");
        return false;
    }

    return true;
}

bool Renderer::CreateDescriptionSetLayout()
{
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;

    vk::DescriptorPoolSize descriptorViewProjectionPoolSize;
    descriptorViewProjectionPoolSize.type = vk::DescriptorType::eUniformBuffer;
    descriptorViewProjectionPoolSize.descriptorCount = 1;

    vk::DescriptorPoolSize descriptorModelPoolSize;
    descriptorModelPoolSize.type = vk::DescriptorType::eUniformBufferDynamic;
    descriptorModelPoolSize.descriptorCount = 1;

    vk::DescriptorPoolSize descriptorSamplerPoolSize;
    descriptorSamplerPoolSize.type = vk::DescriptorType::eCombinedImageSampler;
    descriptorSamplerPoolSize.descriptorCount = m_physicalDeviceProperties.limits.maxDescriptorSetSampledImages;

    descriptorPoolSizes.push_back(descriptorViewProjectionPoolSize);
    descriptorPoolSizes.push_back(descriptorModelPoolSize);
    descriptorPoolSizes.push_back(descriptorSamplerPoolSize);

    vk::DescriptorPoolCreateInfo poolCreateInfo;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    poolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
    poolCreateInfo.maxSets = 3000; //TODO: expand 

    vk::Result result = m_vkLogicalDevice.createDescriptorPool(&poolCreateInfo, nullptr, &m_descriptorPool);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create descriptor pool!");
        return false;
    }

    std::vector<vk::DescriptorSetLayoutBinding> mvpSetLayoutBindings;

    vk::DescriptorSetLayoutBinding setViewProjection;
    setViewProjection.descriptorType = vk::DescriptorType::eUniformBuffer;
    setViewProjection.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setViewProjection.binding = 0; //TODO: hardcode descriptor binding
    setViewProjection.descriptorCount = 1;

    vk::DescriptorSetLayoutBinding setModel;
    setModel.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    setModel.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setModel.binding = 1; //TODO: hardcode descriptor binding
    setModel.descriptorCount = 1;

    vk::DescriptorSetLayoutBinding textureSampler;
    textureSampler.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    textureSampler.stageFlags = vk::ShaderStageFlagBits::eFragment;
    textureSampler.binding = 0; //TODO: hardcode descriptor binding
    textureSampler.descriptorCount = 1;

    mvpSetLayoutBindings.push_back(setViewProjection);
    mvpSetLayoutBindings.push_back(setModel);

    vk::DescriptorSetLayoutCreateInfo mvpLayoutInfo;
    mvpLayoutInfo.pBindings = mvpSetLayoutBindings.data();
    mvpLayoutInfo.bindingCount = static_cast<uint32_t>(mvpSetLayoutBindings.size());

    result = m_vkLogicalDevice.createDescriptorSetLayout(&mvpLayoutInfo, nullptr, &m_descriptorSetLayouts[0]);

    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create descriptor set layout!");
        return false;
    }

    vk::DescriptorSetLayoutCreateInfo albedoLayoutInfo;
    albedoLayoutInfo.pBindings = &textureSampler;
    albedoLayoutInfo.bindingCount = 1;

    result = m_vkLogicalDevice.createDescriptorSetLayout(&albedoLayoutInfo, nullptr, &m_descriptorSetLayouts[1]);

    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create descriptor set layout!");
        return false;
    }

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayouts[0];

    result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &m_mvpDescriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't allocate descriptor sets!");
        return false;
    }

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();

    vk::PushConstantRange pushConstanRange;
    pushConstanRange.setSize(sizeof( glm::vec4));
    pushConstanRange.setStageFlags(vk::ShaderStageFlagBits::eVertex);

    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstanRange;

    result = m_vkLogicalDevice.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_pipelineLayout);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create pipeline layout!");
        return false;
    }

    return true;
}

bool Renderer::CreateGraphicsPipeline()
{
    vk::Result result;
    FreeGraphicsPipeline();

    m_shaderProgram = new ShaderProgram(m_vkLogicalDevice, "data/shaders/UberShader.vert.spv", "data/shaders/UberShader.frag.spv");

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

    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = m_depthTestEnabled;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = vk::CompareOp::eLessOrEqual;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.back.failOp = vk::StencilOp::eKeep;
    depthStencil.back.passOp = vk::StencilOp::eKeep;
    depthStencil.back.compareOp = vk::CompareOp::eAlways;
    depthStencil.back.compareMask = 0;
    depthStencil.back.reference = 0;
    depthStencil.back.depthFailOp = vk::StencilOp::eKeep;
    depthStencil.back.writeMask = 0;
    depthStencil.front = depthStencil.back;

    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    if(!m_shaderProgram->IsCreated())
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
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1; // Optional

    //Solid pipeline

    std::tie(result, m_pipelines.solid) = m_vkLogicalDevice.createGraphicsPipeline({}, pipelineInfo);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create solid pipeline.");
        return false;
    }

    // Alpha blended pipeline

    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcColor;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcColor;

    std::tie(result, m_pipelines.blend) = m_vkLogicalDevice.createGraphicsPipeline({}, pipelineInfo);
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't create blend pipeline.");
        return false;
    }

    // Wire frame rendering pipeline
    if(m_deviceFeatures.fillModeNonSolid)
    {
        //rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eLine;

        std::tie(result, m_pipelines.wired) = m_vkLogicalDevice.createGraphicsPipeline({}, pipelineInfo);
        if(result != vk::Result::eSuccess)
        {
            LOG_ERROR("Can't create blend pipeline.");
            return false;
        }
    }

    m_shaderProgram->DestroyShaderModules();

    return true;
}

bool Renderer::CreateFramebuffers()
{
    FreeFrameBuffers();

    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    vk::ImageView attachments[s_swapChainAttachmentsAmount];
    attachments[1] = m_depthImage->GetVkImageView();

    vk::FramebufferCreateInfo framebufferInfo;
    framebufferInfo.renderPass = m_renderPass;
    framebufferInfo.attachmentCount = s_swapChainAttachmentsAmount;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = m_swapChainExtent.width;
    framebufferInfo.height = m_swapChainExtent.height;
    framebufferInfo.layers = 1;

    for(size_t i = 0; i < m_swapChainImageViews.size(); ++i)
    {
        attachments[0] = m_swapChainImageViews[i];
        vk::Result result = m_vkLogicalDevice.createFramebuffer(&framebufferInfo, nullptr, &m_swapChainFramebuffers[i]);

        if(result != vk::Result::eSuccess)
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
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to create command pool!");
        return false;
    }

    return true;
}

bool Renderer::CreateDepthBuffer()
{
    FreeDepthBuffer();

    if(!FindDepthFormat(m_depthImageFormat))
    {
        LOG_ERROR("Not one of desired depth formats are not compatible!");
        return false;
    }
    m_depthImage = new Image(m_vkPhysicalDevice,
                             m_vkLogicalDevice,
                             m_depthImageFormat,
                             vk::ImageUsageFlagBits::eDepthStencilAttachment,
                             m_swapChainExtent.width,
                             m_swapChainExtent.height);
    return m_depthImage->IsInitialized();
}

bool Renderer::CreateCommandBuffers()
{
    FreeCommandBuffers();

    m_commandBuffers.resize(m_swapChainFramebuffers.size());

    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

    vk::Result result = m_vkLogicalDevice.allocateCommandBuffers(&allocInfo, m_commandBuffers.data());

    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Failed to allocate command buffers!");
        return false;
    }

    for(size_t i = 0; i < m_commandBuffers.size(); ++i)
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

        std::array<vk::ClearValue, 2> clearValues = {};
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil.setDepth(1.0f);

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        m_commandBuffers[i].beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

        vk::DeviceSize offsets[] = {0};

        {
            uint32_t j = 0;

            for(auto pVkMesh : m_vkMeshes)
            {
                if(pVkMesh->IsValid())
                {
                    glm::vec4 colorPush({pVkMesh->GetColor(), pVkMesh->IsColored()}); // xyz - color. w - 1.0 enabled color or 0.0 disabled color

                    if(pVkMesh->IsWired())
                    {
                        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelines.wired);
                    }
                    else
                    {
                        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelines.solid);
                    }

                    m_commandBuffers[i].pushConstants(m_pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(colorPush), glm::value_ptr(colorPush));
                    vk::Buffer vertexBuffer[] = {pVkMesh->GetVertexBuffer()};
                    uint32_t dynamicOffset = j * static_cast<uint32_t>(m_dynamicAlignment);
                    m_commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffer, offsets);
                    m_commandBuffers[i].bindIndexBuffer(pVkMesh->GetIndexBuffer(), 0, vk::IndexType::eUint16);

                    std::array<vk::DescriptorSet, 2> descriptorSets;
                    // Set 0: Scene descriptor set containing global matrices
                    descriptorSets[0] = m_mvpDescriptorSet;
                    // Set 1: Per-Material descriptor set containing bound images
                    descriptorSets[1] = m_descriptorSetsCache.at(pVkMesh->GetMaterialHandle());

                    m_commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, descriptorSets.size(), descriptorSets.data(), 1, &dynamicOffset);

                    m_commandBuffers[i].drawIndexed(pVkMesh->IndicesSize(), 1, 0, 0, 0);

                    ++j;
                }
            }
        }

        m_commandBuffers[i].endRenderPass();

        m_commandBuffers[i].end();
    }

    return true;
}

bool Renderer::CreateSemaphores()
{
    vk::SemaphoreCreateInfo semaphoreInfo;
    if(m_vkLogicalDevice.createSemaphore(&semaphoreInfo, {}, &m_imageAvailableSemaphore) == vk::Result::eSuccess &&
        m_vkLogicalDevice.createSemaphore(&semaphoreInfo, {}, &m_renderFinishedSemaphore) == vk::Result::eSuccess)
    {
        return true;
    }

    LOG_ERROR("Failed to create semaphores!");
    return false;
}

bool Renderer::CreatePipelineCache()
{
    vk::PipelineCacheCreateInfo pipelineCacheCreateInfo;
    if(m_vkLogicalDevice.createPipelineCache(&pipelineCacheCreateInfo, nullptr, &pipelineCache) != vk::Result::eSuccess)
    {
        LOG_ERROR( "Can't create pipeline cache!" );
        return false;
    }
    return true;
}

bool Renderer::LoadEngineHelpData()
{
    unicorn::video::Texture texture;
    std::string path = "data/textures/replace_me.jpg";
    if(!texture.Load(path))
    {
        LOG_ERROR( "Can't find texture with path ", path.c_str() );
        return false;
    }
    VkTexture* replaceMeTexture = new VkTexture(m_vkLogicalDevice);

    if(!replaceMeTexture->Create(m_vkPhysicalDevice, m_vkLogicalDevice, m_commandPool, m_graphicsQueue, &texture))
    {
        LOG_ERROR("Can't create 'replace me' texture ", path.c_str());
        return false;
    }

    m_texturesCache.insert({texture.GetId(), replaceMeTexture});

    m_replaceMeTextureHandle = texture.GetId();

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayouts[1];

    vk::DescriptorSet descriptorSet;

    auto result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &descriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't allocate descriptor sets!");
        return false;
    }

    vk::WriteDescriptorSet imageDescriptorSet;
    imageDescriptorSet.setDstSet(descriptorSet);
    imageDescriptorSet.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    imageDescriptorSet.setDescriptorCount(1);
    imageDescriptorSet.setPImageInfo(&m_texturesCache.at(m_replaceMeTextureHandle)->GetDescriptorImageInfo());

    m_vkLogicalDevice.updateDescriptorSets(1, &imageDescriptorSet, 0, nullptr);

    m_descriptorSetsCache.insert({texture.GetId(), descriptorSet});

    texture.Delete();
    return true;
}

bool Renderer::IsDeviceSuitable(const vk::PhysicalDevice& device)
{
    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

    LOG_INFO("Found GPU : %s", deviceProperties.deviceName);
    QueueFamilyIndices indices = FindQueueFamilies(device);
    bool extensionsSupported = CheckDeviceExtensionSupport(device);
    bool swapChainAcceptable = false;

    vk::PhysicalDeviceFeatures deviceFeatures;
    device.getFeatures(&deviceFeatures);

    if(extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport;
        if(!QuerySwapChainSupport(swapChainSupport, device))
        {
            return false;
        }
        swapChainAcceptable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if(deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu &&
        indices.IsComplete() && extensionsSupported && swapChainAcceptable && deviceFeatures.samplerAnisotropy)
    {
        LOG_INFO("Picked as main GPU : %s", deviceProperties.deviceName);
        m_gpuName = deviceProperties.deviceName;
        return true;
    }
    return false;
}

bool Renderer::AllocateMaterial(const Mesh& mesh, VkMesh& vkmesh)
{
    auto& meshMaterial = mesh.GetMaterial();

    uint32_t meshAlbedoHandle = m_replaceMeTextureHandle;

    if(meshMaterial.AlbedoExist())
    {
        meshAlbedoHandle = meshMaterial.GetAlbedo().GetId();

        if(m_texturesCache.find(meshAlbedoHandle) == m_texturesCache.end())
        {
            VkTexture* vkTexture = new VkTexture(m_vkLogicalDevice);
            vkTexture->Create(m_vkPhysicalDevice, m_vkLogicalDevice, m_commandPool, m_graphicsQueue, &meshMaterial.GetAlbedo());
            m_texturesCache.insert({meshAlbedoHandle, vkTexture});

            vk::DescriptorSetAllocateInfo allocInfo;
            allocInfo.descriptorPool = m_descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &m_descriptorSetLayouts[1];

            vk::DescriptorSet descriptorSet;

            auto result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &descriptorSet);

            if(result != vk::Result::eSuccess)
            {
                LOG_ERROR("Can't allocate descriptor sets!");
                return false;
            }

            vk::WriteDescriptorSet imageDescriptorSet;
            imageDescriptorSet.setDstSet(descriptorSet);
            imageDescriptorSet.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
            imageDescriptorSet.setDescriptorCount(1);
            imageDescriptorSet.setPImageInfo(&m_texturesCache.at(meshAlbedoHandle)->GetDescriptorImageInfo());

            m_vkLogicalDevice.updateDescriptorSets(1, &imageDescriptorSet, 0, nullptr);

            m_descriptorSetsCache.insert({meshAlbedoHandle, descriptorSet});
        }
    }

    vkmesh.SetMaterialHandle(meshAlbedoHandle);
    return true;
}

bool Renderer::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const
{
    vk::Result result;
    std::vector<vk::ExtensionProperties> availableExtensions;
    std::tie(result, availableExtensions) = device.enumerateDeviceExtensionProperties();
    if(result != vk::Result::eSuccess)
    {
        LOG_ERROR("Can't enumerate device extension properties.");
        return false;
    }
    auto deviceExtensions = Context::Instance().GetDeviceExtensions();
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for(const auto& extension : availableExtensions)
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

    if(result == vk::Result::eErrorOutOfDateKHR)
    {
        if(!RecreateSwapChain())
        {
            LOG_ERROR("Can't recreate swapchain!");
            return false;
        }
        return true;
    }
    if(result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
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

    m_camera.Frame();
    UpdateUniformBuffer();
    UpdateDynamicUniformBuffer();

    result = m_graphicsQueue.submit(1, &submitInfo, nullptr);

    if(result != vk::Result::eSuccess)
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

    if(result == vk::Result::eErrorOutOfDateKHR)
    {
        RecreateSwapChain();
        return true;
    }
    if(result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        LOG_ERROR("Failed to acquire swap chain image!");
        return false;
    }

    return true;
}
}
}
}
