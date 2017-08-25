/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_VULKAN_RENDERER_HPP
#define UNICORN_VIDEO_VULKAN_RENDERER_HPP

#include <unicorn/video/Renderer.hpp>
#include <unicorn/video/vulkan/VkMesh.hpp>
#include <unicorn/video/vulkan/Image.hpp>
#include <unicorn/video/vulkan/VkTexture.hpp>

#include <vulkan/vulkan.hpp>

#include <list>
#include <vector>
#include <unordered_map>

namespace unicorn
{
namespace system
{
class Manager;
class Window;
}

namespace video
{
namespace vulkan
{
/**
 * @brief Struct for easy check of required queue family indices
 */
struct QueueFamilyIndices
{
    int32_t graphicsFamily = -1;
    int32_t presentFamily = -1;

    /**
     * @brief Checks if all needed family indices are exists.
     * @return true if all required features are available and false if not
     */
    bool IsComplete() const;
};

/**
 * @brief Swapchain creation details
 */
struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

/**
 * @brief Camera data
 */
struct UniformCameraData
{
    glm::mat4 view;
    glm::mat4 proj;
};

/**
 * @brief Struct which holds all models uniform data for sending to shader
 */
struct UniformAllMeshesData
{
    glm::mat4* model = nullptr;
};

class ShaderProgram;
class UniformObject;
class Image;

/**
 * @brief Vulkan renderer backend
 */
class Renderer : public video::Renderer
{
public:
    /**
     * @brief Basic constructor
     * @param manager Describes required extensions
     * @param window Render into
     */
    Renderer(system::Manager& manager, system::Window* window);

    /**
     * @brief Destructor which calls Deinit()
     */
    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer(const Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(const Renderer&& other) = delete;

    bool Init() override;
    void Deinit() override;
    bool Render() override;
    bool RecreateSwapChain();
    bool AddMesh(Mesh* mesh) override;
    bool DeleteMesh(const Mesh* mesh) override;
    void SetDepthTest(bool enabled) override;
private:
    vk::PhysicalDevice m_vkPhysicalDevice;
    vk::Device m_vkLogicalDevice;
    vk::SwapchainKHR m_vkSwapChain;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;
    vk::SurfaceKHR m_vkWindowSurface;
    vk::Format m_swapChainImageFormat;
    vk::Format m_depthImageFormat;
    vk::Extent2D m_swapChainExtent;
    vk::PipelineLayout m_pipelineLayout;
    vk::RenderPass m_renderPass;
    vk::CommandPool m_commandPool;
    vk::Semaphore m_imageAvailableSemaphore;
    vk::Semaphore m_renderFinishedSemaphore;
    vk::DescriptorPool m_descriptorPool;
    vk::PhysicalDeviceProperties m_physicalDeviceProperties;
    std::string m_gpuName;
    std::vector<vk::Image> m_swapChainImages;
    std::vector<vk::ImageView> m_swapChainImageViews;
    std::vector<vk::Framebuffer> m_swapChainFramebuffers;
    std::vector<vk::CommandBuffer> m_commandBuffers;
    vk::PhysicalDeviceFeatures m_deviceFeatures;
    vk::PipelineCache pipelineCache;

    struct
    {
        vk::Pipeline solid;
        vk::Pipeline blend;
        vk::Pipeline wired;
    } m_pipelines;

    std::list<VkMesh*> m_vkMeshes;
    Image* m_depthImage;
    VkTexture* m_replaceMeTexture;

    std::unordered_map<uint32_t, VkTexture*> m_textures;
    std::unordered_map<uint32_t, vk::DescriptorSet> m_descriptorSetsCache;

    std::array<vk::DescriptorSetLayout, 2> m_descriptorSetLayouts; // 0 - mvp, 1 - albedo
    vk::DescriptorSet m_mvpDescriptorSet;

    ShaderProgram* m_shaderProgram;
    Buffer m_uniformViewProjection;
    Buffer m_uniformModel;
    size_t m_dynamicAlignment;
    UniformAllMeshesData m_uniformModelsData;
    UniformCameraData m_uniformCameraData;

    bool m_hasDirtyMeshes;

    static const bool s_enableValidationLayers;
    static const uint32_t s_swapChainAttachmentsAmount;

    static void DeleteVkMesh(VkMesh* pVkMesh);

    void FreeSurface();
    void FreeLogicalDevice();
    void FreeSwapChain();
    void FreeImageViews();
    void FreeDepthBuffer();
    void FreeRenderPass();
    void FreeGraphicsPipeline();
    void FreeFrameBuffers();
    void FreeCommandPool();
    void FreeCommandBuffers();
    void FreeSemaphores();
    void FreeUniforms();
    void FreeDescriptorPoolAndLayouts() const;
    void FreePipelineCache();
    void FreeEngineHelpData();
    void FreeTextures();

    bool PrepareUniformBuffers();
    void UpdateMVPDescriptorSet();
    void UpdateUniformBuffer();
    void UpdateDynamicUniformBuffer();
    void UpdateVkMeshMatrices();
    bool PickPhysicalDevice();

    bool CreateLogicalDevice();
    bool CreateSurface();
    bool CreateDescriptionSetLayout();
    bool CreateSwapChain();
    bool CreateImageViews();
    bool CreateRenderPass();
    bool CreateGraphicsPipeline();
    bool CreateFramebuffers();
    bool CreateCommandPool();
    bool CreateDepthBuffer();
    bool CreateCommandBuffers();
    bool CreateSemaphores();
    bool CreatePipelineCache();
    bool LoadEngineHelpData();

    bool IsDeviceSuitable(const vk::PhysicalDevice& device);
    bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device) const;
    bool Frame();
    void OnMeshReallocated(VkMesh*);
    QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device) const;
    bool FindSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, vk::Format& returnFormat) const;
    bool FindDepthFormat(vk::Format& desiredFormat) const;
    bool HasStencilComponent(vk::Format format) const;
    bool QuerySwapChainSupport(SwapChainSupportDetails& details, const vk::PhysicalDevice& device) const;
    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) const;
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) const;
    // Callbacks for window events
    void OnWindowDestroyed(system::Window* pWindow);
    void OnWindowSizeChanged(system::Window* pWindow, std::pair<int32_t, int32_t> size);
};
}
}
}
#endif // UNICORN_VIDEO_VULKAN_RENDERER_HPP
