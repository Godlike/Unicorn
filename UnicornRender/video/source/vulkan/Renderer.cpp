/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Renderer.hpp>
#include <unicorn/Settings.hpp>
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
#include <unicorn/video/Primitives.hpp>

#include <unicorn/utility/InternalLoggers.hpp>
#include <mule/asset/SimpleStorage.hpp>

#include <glm/gtc/type_ptr.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

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

Renderer::Renderer(system::Manager& manager, system::Window* window, Camera const& camera)
    : video::IRenderer(manager, window, camera)
    , m_pDepthImage(nullptr)
    , m_contextInstance(Context::Instance().GetVkInstance())
    , m_hasDirtyMeshes(false)
{
    m_pWindow->Destroyed.connect(this, &Renderer::OnWindowDestroyed);
    m_pWindow->SizeChanged.connect(this, &Renderer::OnWindowSizeChanged);
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

    LOG_VULKAN->Info("Renderer initialization started.");

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

    UpdateViewProjectionDescriptorSet();

    m_isInitialized = true;

    LOG_VULKAN->Info("Renderer initialized correctly.");

    return true;
}

void Renderer::Deinit()
{
    if(m_isInitialized)
    {
        {
            if(!m_vkMeshes.empty())
            {
                for(auto& pVkMesh : m_vkMeshes)
                {
                    DeleteVkMesh(pVkMesh);
                }

                LOG_VULKAN->Debug("Deleted {} stray vk meshes", static_cast<uint32_t>(m_vkMeshes.size()));

                m_vkMeshes.clear();
            }
        }

        FreeEngineHelpData();
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

        LOG_VULKAN->Info("Render shutdown correctly.");
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

        std::tie(result, presentSupport) = device.getSurfaceSupportKHR(static_cast<uint32_t>(index), m_vkWindowSurface);

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

bool Renderer::FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,
                                   vk::FormatFeatureFlags features, vk::Format& returnFormat) const
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
        LOG_VULKAN->Error("Can't get surface formats khr.");
        return false;
    }
    std::tie(result, details.presentModes) = device.getSurfacePresentModesKHR(m_vkWindowSurface);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't get surface present modes khr.");
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
            ResizeUnifromModelBuffer(nullptr);
            CreateCommandBuffers();

            m_materials.remove_if([](const std::weak_ptr<VkMaterial>& pVkMaterial) { return pVkMaterial.expired(); });
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
    LOG_VULKAN->Info("Window destroyed, deinitializing renderer");

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
        LOG_VULKAN->Error("Can't recreate swapchain!");
    }
}

bool Renderer::RecreateSwapChain()
{
    m_vkLogicalDevice.waitIdle();

    return CreateSwapChain() &&
           CreateImageViews() &&
           CreateDepthBuffer() &&
           CreateRenderPass() &&
           CreateGraphicsPipeline() &&
           CreateFramebuffers() &&
           CreateCommandBuffers();
}


void Renderer::ResizeUnifromModelBuffer(VkMesh* /*vkmesh*/)
{
    m_uniformModel.Destroy();
    auto const nMeshes = m_vkMeshes.size();

    size_t const bufferSize = (nMeshes == 0) ? m_dynamicAlignment : (nMeshes * m_dynamicAlignment);

    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible, bufferSize);

    if(m_uniformModelsData.model)
    {
        utility::AlignedFree(m_uniformModelsData.model);
    }

    m_uniformModelsData.model = static_cast<glm::mat4*>(utility::AlignedAlloc(bufferSize, m_dynamicAlignment));

    UpdateVkMeshMatrices();

    m_uniformModel.Map();
    m_uniformModel.Write(m_uniformModelsData.model);

    UpdateModelDescriptorSet();

    CreateCommandBuffers();
}

void Renderer::OnMeshMaterialUpdated(Mesh* mesh, VkMesh* vkMesh)
{
    AllocateMaterial(*mesh, *vkMesh);
    CreateCommandBuffers();
}

bool Renderer::AddMesh(Mesh* mesh)
{
    assert(nullptr != mesh);

    auto vkmesh = new VkMesh(m_vkLogicalDevice, m_vkPhysicalDevice,
        m_commandPool, m_graphicsQueue, *mesh
        ,m_renderPass, m_pipelineLayout, m_swapChainExtent,
        "data/shaders/UberShader.vert.spv", "data/shaders/UberShader.frag.spv");

    if (!AllocateMaterial(*mesh, *vkmesh))
    {
        LOG_VULKAN->Error("Can't allocate material!");
        return false;
    }
    vkmesh->ReallocatedOnGpu.connect(this, &vulkan::Renderer::ResizeUnifromModelBuffer);
    vkmesh->MaterialUpdated.connect(this, &vulkan::Renderer::OnMeshMaterialUpdated);

    vkmesh->AllocateOnGPU();

    m_vkMeshes.push_back(vkmesh);

    ResizeUnifromModelBuffer(vkmesh);

    return true;
}

bool Renderer::AddText(Text* text)
{
    assert(nullptr != text);
    // const Settings& settings = Settings::Instance();

    // float scale = 0.01;
    // float offsetX = - settings.GetApplicationWidth() / 2.,
    //       offsetY = - settings.GetApplicationHeight() / 2.;

    float offsetX = 0, offsetY = 0;

    std::list<Mesh*> glyphQuads;

    text->TextUpdated.connect(this, &Renderer::OnTextChanged);

    for (auto c : text->GetText())
    {
        stbtt_aligned_quad q;

        stbtt_GetPackedQuad(charInfo.get(), m_fontAtlasRes.x, m_fontAtlasRes.y,
            c - m_firstChar, &offsetX, &offsetY, &q, 1);

        std::vector<Vertex> vertices{{
            {{q.x0, -q.y1, 0}, {q.s0, q.t1}},
            {{q.x0, -q.y0, 0}, {q.s0, q.t0}},
            {{q.x1, -q.y0, 0}, {q.s1, q.t0}},
            {{q.x1, -q.y1, 0}, {q.s1, q.t1}}
        }};

        Mesh* mesh = new unicorn::video::Mesh;
        mesh->SetMeshData(vertices, {0, 1, 2, 0, 2, 3});
        auto fontMaterial = std::make_shared<unicorn::video::Material>();
        mesh->SetMaterial(fontMaterial);

        mesh->TranslateWorld({ text->GetPosition().x, text->GetPosition().y, 0.0 });
        mesh->UpdateTransformMatrix();

        auto vkmesh = new VkMesh(m_vkLogicalDevice, m_vkPhysicalDevice,
            m_commandPool, m_graphicsQueue, *mesh,
            m_renderPass, m_pipelineLayout, m_swapChainExtent,
            "data/shaders/text.vert.spv", "data/shaders/text.frag.spv");

        vkmesh->pMaterial = m_pFontMaterial;
        vkmesh->ReallocatedOnGpu.connect(this, &vulkan::Renderer::ResizeUnifromModelBuffer);
        vkmesh->MaterialUpdated.connect(this, &vulkan::Renderer::OnMeshMaterialUpdated);

        vkmesh->AllocateOnGPU();

        m_vkMeshes.push_back(vkmesh);
        glyphQuads.push_back(mesh);

        ResizeUnifromModelBuffer(vkmesh);
    }

    m_textQuads.emplace(text, glyphQuads);


    return true;
}

bool Renderer::DeleteMesh(const Mesh* pMesh)
{
    assert(nullptr != pMesh);

    auto vkMeshIt = std::find_if(m_vkMeshes.begin(), m_vkMeshes.end(), [=](VkMesh* p) ->bool { return *p == *pMesh; });

    if (vkMeshIt != m_vkMeshes.end())
    {
        DeleteVkMesh(*vkMeshIt);

        m_vkMeshes.erase(vkMeshIt);

        m_hasDirtyMeshes = true;

        return true;
    }

    return false;
}

bool Renderer::DeleteText(const Text* pText)
{
    assert(nullptr != pText);

    return false;
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

void Renderer::OnTextChanged(Text* text)
{
    for(Mesh* mesh : m_textQuads.at(text))
    {
        DeleteMesh(mesh);
    }

    m_textQuads.at(text).clear();

    float offsetX = 0, offsetY = 0;

    for (auto c : text->GetText())
    {
        stbtt_aligned_quad q;

        stbtt_GetPackedQuad(charInfo.get(), m_fontAtlasRes.x, m_fontAtlasRes.y,
            c - m_firstChar, &offsetX, &offsetY, &q, 1);

        std::vector<Vertex> vertices{ {
            { { q.x0, -q.y1, 0 },{ q.s0, q.t1 } },
        { { q.x0, -q.y0, 0 },{ q.s0, q.t0 } },
        { { q.x1, -q.y0, 0 },{ q.s1, q.t0 } },
        { { q.x1, -q.y1, 0 },{ q.s1, q.t1 } }
            } };

        Mesh* mesh = new unicorn::video::Mesh;
        mesh->SetMeshData(vertices, { 0, 1, 2, 0, 2, 3 });
        auto fontMaterial = std::make_shared<unicorn::video::Material>();
        mesh->SetMaterial(fontMaterial);

        mesh->TranslateWorld({ text->GetPosition().x, text->GetPosition().y, 0.0 });
        mesh->UpdateTransformMatrix();

        auto vkmesh = new VkMesh(m_vkLogicalDevice, m_vkPhysicalDevice,
            m_commandPool, m_graphicsQueue, *mesh,
            m_renderPass, m_pipelineLayout, m_swapChainExtent,
            "data/shaders/text.vert.spv", "data/shaders/text.frag.spv");

        vkmesh->pMaterial = m_pFontMaterial;
        vkmesh->ReallocatedOnGpu.connect(this, &vulkan::Renderer::ResizeUnifromModelBuffer);
        vkmesh->MaterialUpdated.connect(this, &vulkan::Renderer::OnMeshMaterialUpdated);

        vkmesh->AllocateOnGPU();

        m_vkMeshes.push_back(vkmesh);
        m_textQuads.at(text).push_back(mesh);

        ResizeUnifromModelBuffer(vkmesh);
    }
}

void Renderer::FreeSurface()
{
    if(m_vkLogicalDevice && m_vkWindowSurface)
    {
        m_contextInstance.destroySurfaceKHR(m_vkWindowSurface);
        m_vkWindowSurface = nullptr;
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
    if(m_vkLogicalDevice && m_vkSwapChain)
    {
        m_vkLogicalDevice.destroySwapchainKHR(m_vkSwapChain);
        m_vkSwapChain = nullptr;
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
    if(m_pDepthImage && m_pDepthImage->IsInitialized())
    {
        delete m_pDepthImage;
        m_pDepthImage = nullptr;
    }
}

void Renderer::FreeRenderPass()
{
    if(m_vkLogicalDevice && m_renderPass)
    {
        m_vkLogicalDevice.destroyRenderPass(m_renderPass);
        m_renderPass = nullptr;
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
    if(m_vkLogicalDevice && !m_commandBuffers.empty())
    {
        m_vkLogicalDevice.freeCommandBuffers(m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
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
    if(m_vkLogicalDevice && pipelineCache)
    {
        m_vkLogicalDevice.destroyPipelineCache(pipelineCache);
    }
}

void Renderer::FreeEngineHelpData()
{
    m_pReplaceMeMaterial.reset();
    m_materials.clear();
}

bool Renderer::PrepareUniformBuffers()
{
    auto uboAlignment = static_cast<size_t>(m_physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
    m_dynamicAlignment = (sizeof(glm::mat4) / uboAlignment) * uboAlignment + ((sizeof(glm::mat4) % uboAlignment) > 0 ? uboAlignment : 0);
    //3D camera uniform
    m_uniformViewProjection.Create(m_vkPhysicalDevice, m_vkLogicalDevice,
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent,
        sizeof(Camera));

    m_uniformViewProjection.Map();
    m_uniformViewProjection.Write(camera);
    uiCamera = new Camera;
    //2D camera uniform
    m_uniformViewProjection2D.Create(m_vkPhysicalDevice, m_vkLogicalDevice,
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent,
        sizeof(Camera));

    m_uniformViewProjection2D.Map();
    m_uniformViewProjection2D.Write(uiCamera);

    m_uniformModel.Create(m_vkPhysicalDevice, m_vkLogicalDevice,
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible,
        m_dynamicAlignment);

    return true;
}

void Renderer::UpdateViewProjectionDescriptorSet()
{
    std::array<vk::WriteDescriptorSet, 2> writeDescrSets;

    vk::WriteDescriptorSet viewProjectionWriteSet;
    viewProjectionWriteSet.dstSet = m_mvpDescriptorSet;
    viewProjectionWriteSet.descriptorType = vk::DescriptorType::eUniformBuffer;
    viewProjectionWriteSet.dstBinding = 0;
    viewProjectionWriteSet.pBufferInfo = &m_uniformViewProjection.GetDescriptorInfo();
    viewProjectionWriteSet.descriptorCount = 1;

    vk::WriteDescriptorSet viewProjectionWriteSet2D;
    viewProjectionWriteSet2D.dstSet = m_2dMvpDescriptorSet;
    viewProjectionWriteSet2D.descriptorType = vk::DescriptorType::eUniformBuffer;
    viewProjectionWriteSet2D.dstBinding = 0;
    viewProjectionWriteSet2D.pBufferInfo = &m_uniformViewProjection2D.GetDescriptorInfo();
    viewProjectionWriteSet2D.descriptorCount = 1;

    writeDescrSets[0] = viewProjectionWriteSet;
    writeDescrSets[1] = viewProjectionWriteSet2D;

    m_vkLogicalDevice.updateDescriptorSets(writeDescrSets.size(), writeDescrSets.data(), 0, nullptr);
}

void Renderer::UpdateModelDescriptorSet() const
{
    vk::WriteDescriptorSet modelWriteSet;
    modelWriteSet.dstSet = m_mvpDescriptorSet;
    modelWriteSet.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    modelWriteSet.dstBinding = 1;
    modelWriteSet.pBufferInfo = &m_uniformModel.GetDescriptorInfo();
    modelWriteSet.descriptorCount = 1;

    m_vkLogicalDevice.updateDescriptorSets(1, &modelWriteSet, 0, nullptr);

    modelWriteSet.dstSet = m_2dMvpDescriptorSet;

    m_vkLogicalDevice.updateDescriptorSets(1, &modelWriteSet, 0, nullptr);
}

void Renderer::UpdateUniformBuffer()
{
    m_uniformViewProjection.Write(camera);
    m_uniformViewProjection2D.Write(uiCamera);
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
    std::tie(result, devices) = m_contextInstance.enumeratePhysicalDevices();
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Failed to enumerate physical devices.");
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
        LOG_VULKAN->Error("Failed to find a suitable GPU!");
        return false;
    }
    m_physicalDeviceProperties = m_vkPhysicalDevice.getProperties();
    return true;
}

bool Renderer::CreateLogicalDevice()
{
    QueueFamilyIndices const indices = FindQueueFamilies(m_vkPhysicalDevice);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    float queuePriority = 1.0f;

    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo const queueCreateInfo({}, queueFamily, 1, &queuePriority);
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
        LOG_VULKAN->Error("Can't initialize Vulkan logical device!");
        return false;
    }
    m_graphicsQueue = m_vkLogicalDevice.getQueue(static_cast<uint32_t>(indices.graphicsFamily), 0);
    m_presentQueue = m_vkLogicalDevice.getQueue(static_cast<uint32_t>(indices.presentFamily), 0);

    return true;
}

bool Renderer::CreateSurface()
{
    if(!m_pWindow || m_systemManager.CreateVulkanSurfaceForWindow(*m_pWindow, m_contextInstance, nullptr,
                                                                  reinterpret_cast<VkSurfaceKHR*>(&m_vkWindowSurface)) != VK_SUCCESS)
    {
        LOG_VULKAN->Error("Failed to create window surface!");

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
        LOG_VULKAN->Error("Failed to create Vulkan swap chain!");
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
        LOG_VULKAN->Error("SwapChain images not equal!");
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
            LOG_VULKAN->Error("Failed to create image views!");
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
        LOG_VULKAN->Error("Failed to create render pass!");
        return false;
    }

    return true;
}

bool Renderer::CreateDescriptionSetLayout()
{
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;

    vk::DescriptorPoolSize descriptorViewProjectionPoolSize;
    descriptorViewProjectionPoolSize.type = vk::DescriptorType::eUniformBuffer;
    descriptorViewProjectionPoolSize.descriptorCount = 2;

    vk::DescriptorPoolSize descriptorModelPoolSize;
    descriptorModelPoolSize.type = vk::DescriptorType::eUniformBufferDynamic;
    descriptorModelPoolSize.descriptorCount = 2;

    vk::DescriptorPoolSize descriptorSamplerPoolSize;
    descriptorSamplerPoolSize.type = vk::DescriptorType::eCombinedImageSampler;
    descriptorSamplerPoolSize.descriptorCount = 3000; //TODO: task [#101] Custom vulkan allocator must enhance this

    descriptorPoolSizes.push_back(descriptorViewProjectionPoolSize);
    descriptorPoolSizes.push_back(descriptorModelPoolSize);
    descriptorPoolSizes.push_back(descriptorSamplerPoolSize);

    vk::DescriptorPoolCreateInfo poolCreateInfo;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    poolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
    poolCreateInfo.maxSets = 3000; //TODO: task [#101] Custom vulkan allocator must enhance this
    poolCreateInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;

    vk::Result result = m_vkLogicalDevice.createDescriptorPool(&poolCreateInfo, nullptr, &m_descriptorPool);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create descriptor pool!");
        return false;
    }

    std::vector<vk::DescriptorSetLayoutBinding> mvpSetLayoutBindings;

    vk::DescriptorSetLayoutBinding setViewProjection;
    setViewProjection.descriptorType = vk::DescriptorType::eUniformBuffer;
    setViewProjection.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setViewProjection.binding = 0;
    setViewProjection.descriptorCount = 1;

    vk::DescriptorSetLayoutBinding setModel;
    setModel.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
    setModel.stageFlags = vk::ShaderStageFlagBits::eVertex;
    setModel.binding = 1;
    setModel.descriptorCount = 1;

    vk::DescriptorSetLayoutBinding textureSampler;
    textureSampler.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    textureSampler.stageFlags = vk::ShaderStageFlagBits::eFragment;
    textureSampler.binding = 0;
    textureSampler.descriptorCount = 1;

    mvpSetLayoutBindings.push_back(setViewProjection);
    mvpSetLayoutBindings.push_back(setModel);

    vk::DescriptorSetLayoutCreateInfo mvpLayoutInfo;
    mvpLayoutInfo.pBindings = mvpSetLayoutBindings.data();
    mvpLayoutInfo.bindingCount = static_cast<uint32_t>(mvpSetLayoutBindings.size());

    result = m_vkLogicalDevice.createDescriptorSetLayout(&mvpLayoutInfo, nullptr, &m_descriptorSetLayouts[0]);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create descriptor set layout!");
        return false;
    }

    vk::DescriptorSetLayoutCreateInfo albedoLayoutInfo;
    albedoLayoutInfo.pBindings = &textureSampler;
    albedoLayoutInfo.bindingCount = 1;

    result = m_vkLogicalDevice.createDescriptorSetLayout(&albedoLayoutInfo, nullptr, &m_descriptorSetLayouts[1]);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create descriptor set layout!");
        return false;
    }

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayouts[0];

    result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &m_mvpDescriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't allocate descriptor sets!");
        return false;
    }

    vk::DescriptorSetAllocateInfo allocInfo2D;
    allocInfo2D.descriptorPool = m_descriptorPool;
    allocInfo2D.descriptorSetCount = 1;
    allocInfo2D.pSetLayouts = &m_descriptorSetLayouts[0];

    result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo2D, &m_2dMvpDescriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't allocate descriptor sets!");
        return false;
    }

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();

    vk::PushConstantRange pushConstanRange;
    pushConstanRange.setSize(sizeof(glm::vec4) * 2); // color and texture coordinates
    pushConstanRange.setStageFlags(vk::ShaderStageFlagBits::eVertex);

    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstanRange;

    result = m_vkLogicalDevice.createPipelineLayout(&pipelineLayoutInfo, nullptr, &m_pipelineLayout);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Failed to create pipeline layout!");
        return false;
    }

    return true;
}

bool Renderer::CreateGraphicsPipeline()
{
    vk::Result result;
    FreeGraphicsPipeline();

    m_shaderProgram = new ShaderProgram(m_vkLogicalDevice,
        "data/shaders/UberShader.vert.spv",
        "data/shaders/UberShader.frag.spv");

    if(!m_shaderProgram->IsCreated())
    {
        LOG_VULKAN->Error("Vulkan can't create shader program!");
        return false;
    }

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
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
                                          vk::ColorComponentFlagBits::eG |
                                          vk::ColorComponentFlagBits::eB |
                                          vk::ColorComponentFlagBits::eA;

    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
    colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    if(!m_shaderProgram->IsCreated())
    {
        LOG_VULKAN->Error("Can't create shader module!");
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
        LOG_VULKAN->Error("Can't create solid pipeline.");
        return false;
    }

    // Wire frame rendering pipeline
    if(m_deviceFeatures.fillModeNonSolid)
    {
        colorBlendAttachment.blendEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eLine;

        std::tie(result, m_pipelines.wired) = m_vkLogicalDevice.createGraphicsPipeline({}, pipelineInfo);
        if(result != vk::Result::eSuccess)
        {
            LOG_VULKAN->Error("Can't create blend pipeline.");
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
    attachments[1] = m_pDepthImage->GetVkImageView();

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
            LOG_VULKAN->Error("Failed to create framebuffer!");
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
        LOG_VULKAN->Error("Failed to create command pool!");
        return false;
    }

    return true;
}

bool Renderer::CreateDepthBuffer()
{
    FreeDepthBuffer();

    if(!FindDepthFormat(m_depthImageFormat))
    {
        LOG_VULKAN->Error("Not one of desired depth formats are not compatible!");
        return false;
    }
    m_pDepthImage = new Image(m_vkPhysicalDevice,
                              m_vkLogicalDevice,
                              m_depthImageFormat,
                              vk::ImageUsageFlagBits::eDepthStencilAttachment,
                              m_swapChainExtent.width,
                              m_swapChainExtent.height);
    return m_pDepthImage->IsInitialized();
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
        LOG_VULKAN->Error("Failed to allocate command buffers!");
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
                auto vkMeshMaterial = pVkMesh->GetMesh().GetMaterial();

                if (!vkMeshMaterial->IsVisible())
                {
                    ++j;
                    continue;
                }

                if(pVkMesh->IsValid())
                {
                    glm::vec4 colorPush(
                        vkMeshMaterial->GetColor(), // xyz - color
                        vkMeshMaterial->IsColored() // w - boolean flag for 1 enabled color or 0 disabled color
                    );

                    m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pVkMesh->m_pipeline);
                    m_commandBuffers[i].pushConstants(m_pipelineLayout,
                        vk::ShaderStageFlagBits::eVertex, 0,
                        sizeof(glm::vec4), glm::value_ptr(colorPush));

                    m_commandBuffers[i].pushConstants(m_pipelineLayout,
                        vk::ShaderStageFlagBits::eVertex, sizeof(glm::vec4),
                        sizeof(glm::vec4), glm::value_ptr(
                            vkMeshMaterial->GetNormalizedSpriteArea()));

                    vk::Buffer vertexBuffer[] = {pVkMesh->GetVertexBuffer()};
                    uint32_t dynamicOffset = j * static_cast<uint32_t>(m_dynamicAlignment);
                    m_commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffer, offsets);
                    m_commandBuffers[i].bindIndexBuffer(pVkMesh->GetIndexBuffer(), 0, vk::IndexType::eUint32);

                    m_commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout,
                        0, static_cast<uint32_t>(pVkMesh->pMaterial->descriptorSets.size()),
                        pVkMesh->pMaterial->descriptorSets.data(), 1, &dynamicOffset);

                    m_commandBuffers[i].drawIndexed(
                        static_cast<uint32_t>(
                            pVkMesh->GetMesh().GetIndices().size()), 1, 0, 0, 0);

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

    LOG_VULKAN->Error("Failed to create semaphores!");
    return false;
}

bool Renderer::CreatePipelineCache()
{
    vk::PipelineCacheCreateInfo pipelineCacheCreateInfo;
    if(m_vkLogicalDevice.createPipelineCache(&pipelineCacheCreateInfo, nullptr, &pipelineCache) != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error( "Can't create pipeline cache!" );
        return false;
    }
    return true;
}

bool Renderer::LoadEngineHelpData()
{
    Texture texture;
    static std::string const path = "data/textures/replace_me.jpg";
    if(!texture.Load(path))
    {
        LOG_VULKAN->Error( "Can't find texture with path - {}", path.c_str() );
        return false;
    }
    VkTexture* replaceMeTexture = new VkTexture(m_vkLogicalDevice);

    if(!replaceMeTexture->Create(m_vkPhysicalDevice, m_vkLogicalDevice,
        m_commandPool, m_graphicsQueue, texture))
    {
        LOG_VULKAN->Error("Can't create 'replace me' texture - {}", path.c_str());

        delete replaceMeTexture;

        return false;
    }

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayouts[1];

    vk::DescriptorSet descriptorSet;

    auto result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &descriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't allocate descriptor sets!");

        replaceMeTexture->Delete();
        delete replaceMeTexture;

        return false;
    }

    vk::WriteDescriptorSet imageDescriptorSet;
    imageDescriptorSet.setDstSet(descriptorSet);
    imageDescriptorSet.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    imageDescriptorSet.setDescriptorCount(1);
    imageDescriptorSet.setPImageInfo(&replaceMeTexture->GetDescriptorImageInfo());

    m_vkLogicalDevice.updateDescriptorSets(1, &imageDescriptorSet, 0, nullptr);

    m_pReplaceMeMaterial = std::shared_ptr<VkMaterial>(new VkMaterial, [](VkMaterial* p)
    {
        p->texture->Delete();
        p->device.freeDescriptorSets(p->pool,
            p->descriptorSets.size(),
            p->descriptorSets.data());
        delete p;
    });

    m_pReplaceMeMaterial->texture = replaceMeTexture;
    m_pReplaceMeMaterial->descriptorSets.push_back(m_mvpDescriptorSet);
    m_pReplaceMeMaterial->descriptorSets.push_back(descriptorSet);
    m_pReplaceMeMaterial->handle = texture.GetId();
    m_pReplaceMeMaterial->device = m_vkLogicalDevice;
    m_pReplaceMeMaterial->pool = m_descriptorPool;

    m_materials.push_back(m_pReplaceMeMaterial);

    // Loading font
    std::string const fontPath = "data/fonts/consolas.ttf";
    auto& storage = mule::asset::SimpleStorage::Instance();
    mule::asset::Handler fontHandler = storage.Get(fontPath);

    if (!fontHandler.IsValid())
    {
        LOG_VIDEO->Error("Can't find font - {}", fontPath.c_str());
        return false;
    }

    uint8_t* atlasData = new uint8_t[m_fontAtlasRes.x * m_fontAtlasRes.y]; // TODO: free

    stbtt_pack_context context;

    if (!stbtt_PackBegin(&context, atlasData, m_fontAtlasRes.x, m_fontAtlasRes.y, 0, 1, nullptr))
    {
        LOG_VIDEO->Error("Failed to initialize font");

        return false;
    }

    stbtt_PackSetOversampling(&context, m_fontOversample.x, m_fontOversample.y);

    charInfo = std::make_unique<stbtt_packedchar[]>(m_charCount);

    if (!stbtt_PackFontRange(&context, fontHandler.GetContent().GetBuffer().data(),
        0, m_fontSize, m_firstChar, m_charCount, charInfo.get()))
    {
        LOG_VIDEO->Error("Failed to pack font");

        return false;
    }

    stbtt_PackEnd(&context);

    Texture fontTexture;

    if(!fontTexture.Load(atlasData, m_fontAtlasRes.x, m_fontAtlasRes.y))
    {
        LOG_VULKAN->Error( "Can't load font texture" );
        return false;
    }

    VkTexture* atlasVkTexture = new VkTexture(m_vkLogicalDevice, vk::Format::eR8Unorm);

    if(!atlasVkTexture->Create(m_vkPhysicalDevice, m_vkLogicalDevice,
        m_commandPool, m_graphicsQueue, fontTexture))
    {
        LOG_VULKAN->Error("Can't allocate font atlas texture");

        delete atlasVkTexture;

        return false;
    }

    vk::DescriptorSetAllocateInfo fontAllocInfo;
    fontAllocInfo.descriptorPool = m_descriptorPool;
    fontAllocInfo.descriptorSetCount = 1;
    fontAllocInfo.pSetLayouts = &m_descriptorSetLayouts[1];

    vk::DescriptorSet fontDescriptorSet;

    result = m_vkLogicalDevice.allocateDescriptorSets(&fontAllocInfo, &fontDescriptorSet);

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't allocate descriptor sets!");

        atlasVkTexture->Delete();
        delete atlasVkTexture;

        return false;
    }

    vk::WriteDescriptorSet fontImageDescriptorSet;
    fontImageDescriptorSet.setDstSet(fontDescriptorSet);
    fontImageDescriptorSet.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    fontImageDescriptorSet.setDescriptorCount(1);
    fontImageDescriptorSet.setPImageInfo(&atlasVkTexture->GetDescriptorImageInfo());

    m_vkLogicalDevice.updateDescriptorSets(1, &fontImageDescriptorSet, 0, nullptr);

    m_pFontMaterial = std::shared_ptr<VkMaterial>(new VkMaterial, [](VkMaterial* p)
    {
        p->texture->Delete();
        p->device.freeDescriptorSets(p->pool,
            p->descriptorSets.size(),
            p->descriptorSets.data());
        delete p;
    });

    m_pFontMaterial->texture = atlasVkTexture;
    m_pFontMaterial->descriptorSets.push_back(m_2dMvpDescriptorSet);
    m_pFontMaterial->descriptorSets.push_back(fontDescriptorSet);
    m_pFontMaterial->handle = texture.GetId();
    m_pFontMaterial->device = m_vkLogicalDevice;
    m_pFontMaterial->pool = m_descriptorPool;

    m_materials.push_back(m_pFontMaterial);

    return true;
}

bool Renderer::IsDeviceSuitable(const vk::PhysicalDevice& device)
{
    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

    LOG_VULKAN->Info("Found GPU : {}", deviceProperties.deviceName);
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

    if(indices.IsComplete() && extensionsSupported && swapChainAcceptable && deviceFeatures.samplerAnisotropy)
    {
        LOG_VULKAN->Info("Picked as main GPU : {}", deviceProperties.deviceName);
        m_gpuName = deviceProperties.deviceName;
        return true;
    }
    return false;
}

bool Renderer::AllocateMaterial(const Mesh& mesh, VkMesh& vkmesh)
{
    auto meshMaterial = mesh.GetMaterial();

    if(meshMaterial->GetAlbedo() != nullptr)
    {
        uint32_t meshAlbedoHandle = meshMaterial->GetAlbedo()->GetId();

        auto meshMaterialIt = std::find_if(m_materials.begin(), m_materials.end(), [=](std::weak_ptr<VkMaterial> material) ->bool { return material.lock()->handle == meshAlbedoHandle; });

        if(meshMaterialIt == m_materials.end())
        {
            VkTexture* vkTexture = new VkTexture(m_vkLogicalDevice);
            vkTexture->Create(m_vkPhysicalDevice, m_vkLogicalDevice, m_commandPool, m_graphicsQueue, *meshMaterial->GetAlbedo().get());

            if(!vkTexture->IsInitialized())
            {
                LOG_VULKAN->Error("Can't allocate vulkan texture!");

                delete vkTexture;

                return false;
            }

            vk::DescriptorSetAllocateInfo allocInfo;
            allocInfo.descriptorPool = m_descriptorPool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &m_descriptorSetLayouts[1];

            vk::DescriptorSet descriptorSet;

            auto result = m_vkLogicalDevice.allocateDescriptorSets(&allocInfo, &descriptorSet);

            if(result != vk::Result::eSuccess)
            {
                LOG_VULKAN->Error("Can't allocate sampler descriptor sets!");

                return false;
            }

            vk::WriteDescriptorSet imageDescriptorSet;
            imageDescriptorSet.setDstSet(descriptorSet);
            imageDescriptorSet.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
            imageDescriptorSet.setDescriptorCount(1);
            imageDescriptorSet.setPImageInfo(&vkTexture->GetDescriptorImageInfo());

            m_vkLogicalDevice.updateDescriptorSets(1, &imageDescriptorSet, 0, nullptr);

            vkmesh.pMaterial = std::shared_ptr<VkMaterial>(new VkMaterial, [](VkMaterial* p)
                                                   {
                                                       p->texture->Delete();
                                                       p->device.freeDescriptorSets(p->pool,
                                                                p->descriptorSets.size(),
                                                                p->descriptorSets.data());
                                                       delete p;
                                                   });

            vkmesh.pMaterial->texture = vkTexture;
            vkmesh.pMaterial->descriptorSets.push_back(m_mvpDescriptorSet);
            vkmesh.pMaterial->descriptorSets.push_back(descriptorSet);
            vkmesh.pMaterial->handle = meshAlbedoHandle;
            vkmesh.pMaterial->device = m_vkLogicalDevice;
            vkmesh.pMaterial->pool = m_descriptorPool;

            m_materials.push_back(vkmesh.pMaterial);
        }
        else
        {
            vkmesh.pMaterial = (*meshMaterialIt).lock();
        }
    }
    else
    {
        vkmesh.pMaterial = m_pReplaceMeMaterial;
    }

    return true;
}

bool Renderer::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device)
{
    vk::Result result;
    std::vector<vk::ExtensionProperties> availableExtensions;
    std::tie(result, availableExtensions) = device.enumerateDeviceExtensionProperties();
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't enumerate device extension properties.");
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
            LOG_VULKAN->Error("Can't recreate swapchain!");
            return false;
        }
        return true;
    }
    if(result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        LOG_VULKAN->Error("Failed to acquire swap chain image!");
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

    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("failed to submit draw command buffer!");
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
        LOG_VULKAN->Error("Failed to acquire swap chain image!");
        return false;
    }

    return true;
}
}
}
}
