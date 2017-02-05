/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <vorpal/system/Settings.hpp>
#include <vorpal/utility/Logger.hpp>
#include <vorpal/utility/asset/SimpleStorage.hpp>
#include <vorpal/graphics/VulkanRenderer.hpp>
#include <vorpal/graphics/Vertex.hpp>
#include <vorpal/graphics/VulkanUtils.hpp>

#include <algorithm>
#include <iostream>
#include <set>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //Vulkan depth 0.0 to 1.0 instead -1.0 to 0.0
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace vp
{
namespace graphics
{
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/,
    size_t /*location*/,
    int32_t /*code*/,
    const char* /*layerPrefix*/,
    const char* msg,
    void* /*userData*/)
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
    return VK_FALSE;
}

VulkanRenderer::VulkanRenderer()
    : m_isInitialized(false)
    , m_pWindow(VK_NULL_HANDLE)
    , m_vkInstance(VK_NULL_HANDLE)
    , m_vkPhysicalDevice(nullptr)
    , m_swapChainImageFormat()
    , m_validationLayers({"VK_LAYER_LUNARG_standard_validation"})
    , m_deviceExtensions({VK_KHR_SWAPCHAIN_EXTENSION_NAME})
{
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
    m_descriptorPool = VK_NULL_HANDLE;
    m_graphicsQueue = VK_NULL_HANDLE;
    m_presentQueue = VK_NULL_HANDLE;
    m_descriptorSet = VK_NULL_HANDLE;
    m_textureImage = VK_NULL_HANDLE;
    m_textureImageMemory = VK_NULL_HANDLE;
    m_textureImageView = VK_NULL_HANDLE;
    m_textureSampler = VK_NULL_HANDLE;
    m_depthImageMemory = VK_NULL_HANDLE;
    m_depthImageView = VK_NULL_HANDLE;
    m_depthImage = VK_NULL_HANDLE;
}

VulkanRenderer::~VulkanRenderer()
{
    Deinit();
}

bool VulkanRenderer::Init()
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

    const auto& settings = system::Settings::Instance();

    m_pWindow = glfwCreateWindow(settings.GetApplicationWidth(),
        settings.GetApplicationHeight(),
        settings.GetApplicationName().c_str(),
        nullptr,
        nullptr);
    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowSizeCallback(m_pWindow, onWindowResized);
    if (!CreateInstance() || !SetupDebugCallback() || !CreateSurface() || !PickPhysicalDevice() || !CreateLogicalDevice() || !CreateSwapChain() || !CreateImageViews() || !CreateRenderPass() || !CreateDescriptorSetLayout() || !CreateGraphicsPipeline() || !CreateCommandPool() || !CreateDepthResources() || !CreateFramebuffers() || !CreateTextureImage() || !CreateTextureImageView() || !CreateTextureSampler() || !LoadModel() || !CreateVertexBuffer() || !CreateIndexBuffer() || !CreateUniformBuffer() || !CreateDescriptorPool() || !CreateDescriptorSet() || !CreateCommandBuffers() || !CreateSemaphores())
        return false;
    m_timer.Start();
    m_isInitialized = true;

    LOG_INFO("Vulkan render initialized correctly.");

    return true;
}

void VulkanRenderer::Deinit()
{
    if (m_depthImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_depthImageMemory, nullptr);
        m_depthImageMemory = VK_NULL_HANDLE;
    }

    if (m_depthImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(m_vkLogicalDevice, m_depthImageView, nullptr);
        m_depthImageView = VK_NULL_HANDLE;
    }

    if (m_depthImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(m_vkLogicalDevice, m_depthImage, nullptr);
        m_depthImage = VK_NULL_HANDLE;
    }

    if (m_textureSampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(m_vkLogicalDevice, m_textureSampler, nullptr);
        m_textureSampler = VK_NULL_HANDLE;
    }
    if (m_textureImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(m_vkLogicalDevice, m_textureImageView, nullptr);
        m_textureImageView = VK_NULL_HANDLE;
    }
    if (m_textureImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_textureImageMemory, VK_NULL_HANDLE);
        m_textureImageMemory = VK_NULL_HANDLE;
    }

    if (m_textureImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(m_vkLogicalDevice, m_textureImage, nullptr);
        m_textureImage = VK_NULL_HANDLE;
    }

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

    for (VkImageView& view : m_swapChainImageViews)
    {
        vkDestroyImageView(m_vkLogicalDevice, view, nullptr);
        view = VK_NULL_HANDLE;
    }
    m_swapChainImageViews.clear();

    for (VkFramebuffer& framebuffer : m_swapChainFramebuffers)
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
    if (m_isInitialized)
    {
        LOG_INFO("Vulkan render shutdown correctly.");
    }
    m_isInitialized = false;
}

QueueFamilyIndices VulkanRenderer::FindQueueFamilies(VkPhysicalDevice device) const
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
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

SwapChainSupportDetails VulkanRenderer::QuerySwapChainSupport(VkPhysicalDevice device) const
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

VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
    if (availableFormats.size() == 1 &&
        availableFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanRenderer::ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities)
{
    const system::Settings& settings = system::Settings::Instance();

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

bool VulkanRenderer::FindMemoryType(
    uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t& memoryType)
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

void VulkanRenderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);

    vkFreeCommandBuffers(m_vkLogicalDevice, m_commandPool, 1, &commandBuffer);
}

void VulkanRenderer::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (HasStencilComponent(format))
        {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else
    {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    else
    {
        LOG_ERROR("Unsupported layout transition!");
        //TODO: return false here?
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier);

    EndSingleTimeCommands(commandBuffer);
}

void VulkanRenderer::CopyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageSubresourceLayers subResource = {};
    subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subResource.baseArrayLayer = 0;
    subResource.mipLevel = 0;
    subResource.layerCount = 1;

    VkImageCopy region = {};
    region.srcSubresource = subResource;
    region.dstSubresource = subResource;
    region.srcOffset = {0, 0, 0};
    region.dstOffset = {0, 0, 0};
    region.extent.width = width;
    region.extent.height = height;
    region.extent.depth = 1;

    vkCmdCopyImage(
        commandBuffer,
        srcImage,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dstImage,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    EndSingleTimeCommands(commandBuffer);
}

VkResult VulkanRenderer::CreateDebugReportCallbackEXT(
    const VkDebugReportCallbackCreateInfoEXT* pCreateInfo)
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

void VulkanRenderer::DestroyDebugReportCallbackEXT() const
{
    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT"));
    if (func != nullptr)
    {
        func(m_vkInstance, m_vulkanCallback, nullptr);
    }
}

void VulkanRenderer::waitAsyncEnd() const
{
    vkDeviceWaitIdle(m_vkLogicalDevice);
}

VkCommandBuffer VulkanRenderer::BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_vkLogicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanRenderer::Render()
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
                    system::Settings::Instance().GetApplicationName().c_str());
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

void VulkanRenderer::onWindowResized(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0)
        return;
    VulkanRenderer* VulkanRenderer =
        reinterpret_cast<class VulkanRenderer*>(glfwGetWindowUserPointer(window));
    if (!VulkanRenderer->RecreateSwapChain())
        LOG_ERROR("Can't recreate swapchain!");
}

bool VulkanRenderer::RecreateSwapChain()
{
    waitAsyncEnd();

    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateDepthResources();
    CreateFramebuffers();
    CreateCommandBuffers();
    return true;
}

bool VulkanRenderer::CreateInstance()
{
    if (s_enableValidationLayers && !CheckValidationLayerSupport(m_validationLayers))
    {
        LOG_ERROR("Vulkan validation layers requested, but not available!");
        return false;
    }

    const system::Settings& settings = system::Settings::Instance();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = settings.GetApplicationName().c_str();
    appInfo.applicationVersion =
        VK_MAKE_VERSION(1, 0, 0); // TODO: remove hardcode
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

bool VulkanRenderer::PickPhysicalDevice()
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

    for (const auto& device : devices)
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

bool VulkanRenderer::CreateLogicalDevice()
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

    VkPhysicalDeviceFeatures deviceFeatures = {}; // No features for now.

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

bool VulkanRenderer::CreateSurface()
{
    if (glfwCreateWindowSurface(
            m_vkInstance, m_pWindow, nullptr, &m_vkWindowSurface) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create window surface!");
        return false;
    }
    return true;
}

bool VulkanRenderer::CreateSwapChain()
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
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT
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
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
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

bool VulkanRenderer::CreateImageViews()
{
    for (VkImageView& view : m_swapChainImageViews)
    {
        vkDestroyImageView(m_vkLogicalDevice, view, nullptr);
    }
    m_swapChainImageViews.resize(m_swapChainImages.size());
    for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
    {
        CreateImageView(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, m_swapChainImageViews[i]);
    }
    return true;
}

bool VulkanRenderer::CreateRenderPass()
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

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = VK_FORMAT_D32_SFLOAT;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {{colorAttachment, depthAttachment}};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(
            m_vkLogicalDevice, &renderPassInfo, nullptr, &m_renderPass) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create render pass!");
        return false;
    }

    return true;
}

bool VulkanRenderer::CreateGraphicsPipeline()
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

    vp::utility::asset::SimpleStorage& storage =
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

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {};  // Optional

    VkViewport viewport =
        {}; // TODO : if we will using super sampling. Need to rethink.
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
        {}; // TODO: configure MSAA
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
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1; // Optional

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

bool VulkanRenderer::CreateFramebuffers()
{
    for (auto& framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_vkLogicalDevice, framebuffer, nullptr);
    }
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());
    for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
    {
        for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments = {{m_swapChainImageViews[i],
                m_depthImageView}};

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
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
    }
    return true;
}

bool VulkanRenderer::CreateCommandPool()
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

bool VulkanRenderer::CreateDepthResources()
{
    if (m_depthImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, m_depthImageMemory, nullptr);
        m_depthImageMemory = VK_NULL_HANDLE;
    }

    if (m_depthImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(m_vkLogicalDevice, m_depthImageView, nullptr);
        m_depthImageView = VK_NULL_HANDLE;
    }

    if (m_depthImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(m_vkLogicalDevice, m_depthImage, nullptr);
        m_depthImage = VK_NULL_HANDLE;
    }

    VkFormat depthFormat = VK_FORMAT_D32_SFLOAT; //TODO: make search for supported format here.
    CreateImage(m_swapChainExtent.width, m_swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
    CreateImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, m_depthImageView);
    TransitionImageLayout(m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    return true;
}

bool VulkanRenderer::CreateCommandBuffers()
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
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

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

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

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
            VK_INDEX_TYPE_UINT32);
        // UINTxxx
        vkCmdBindDescriptorSets(m_commandBuffers[i],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipelineLayout,
            0,
            1,
            &m_descriptorSet,
            0,
            nullptr);
        vkCmdDrawIndexed(m_commandBuffers[i], static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
        vkCmdEndRenderPass(m_commandBuffers[i]);

        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
        {
            LOG_ERROR("Failed to record command buffer!");
            return false;
        }
    }
    return true;
}

bool VulkanRenderer::CreateSemaphores()
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

bool VulkanRenderer::CreateVertexBuffer()
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
    void* data;

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

bool VulkanRenderer::CreateIndexBuffer()
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
    void* data;
    vkMapMemory(
        m_vkLogicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_indices.data(), static_cast<size_t>(bufferSize));
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

bool VulkanRenderer::CreateUniformBuffer()
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

void VulkanRenderer::UpdateUniformBuffer()
{
    auto time = m_timer.Seconds();
    UniformBufferObject ubo = {};
    ubo.model = glm::rotate(
        glm::mat4(), time * glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f),
        m_swapChainExtent.width / static_cast<float>(m_swapChainExtent.height),
        0.1f,
        10.0f);
    ubo.proj[1][1] *= -1;

    void* data;
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

bool VulkanRenderer::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {{uboLayoutBinding, samplerLayoutBinding}};
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(
            m_vkLogicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout) !=
        VK_SUCCESS)
    {
        LOG_ERROR("failed to create descriptor set layout!");
        return false;
    }
    return true;
}

bool VulkanRenderer::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
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

bool VulkanRenderer::CreateDescriptorSet()
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

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_textureImageView;
    imageInfo.sampler = m_textureSampler;

    std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = m_descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = m_descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(m_vkLogicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    return true;
}

bool VulkanRenderer::LoadModel()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, m_modelPath.c_str()))
    {
        LOG_ERROR("%s", err.c_str());
        return false;
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex = {};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};
            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            m_vertices.push_back(vertex);
            m_indices.push_back(static_cast<unsigned int>(m_indices.size()));
        }
    }
    return true;
}

bool VulkanRenderer::CreateTextureImage()
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;
    stbi_uc* pixels = nullptr;

    {
        vp::utility::asset::SimpleStorage& storage = vp::utility::asset::SimpleStorage::Instance();
        vp::utility::asset::Handler textureHandler = storage.Get(m_texturePath.c_str());

        if (!textureHandler.IsValid())
        {
            LOG_ERROR("Failed to load asset! Path: %s", m_texturePath.c_str());
            return false;
        }

        const std::vector<uint8_t>& textureContent = textureHandler.GetContent().GetBuffer();

        pixels = stbi_load_from_memory(textureContent.data(), static_cast<int>(textureContent.size()), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        LOG_ERROR("Failed to load texture image!");
        return false;
    }

    VkImage stagingImage = VK_NULL_HANDLE;
    VkDeviceMemory stagingImageMemory = VK_NULL_HANDLE;

    if (!CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage, stagingImageMemory))
        return false;

    VkImageSubresource subresource = {};
    subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource.mipLevel = 0;
    subresource.arrayLayer = 0;

    VkSubresourceLayout stagingImageLayout;
    vkGetImageSubresourceLayout(m_vkLogicalDevice, stagingImage, &subresource, &stagingImageLayout);

    void* data;
    vkMapMemory(m_vkLogicalDevice, stagingImageMemory, 0, imageSize, 0, &data);

    if (stagingImageLayout.rowPitch == static_cast<VkDeviceSize>(texWidth * 4))
    {
        memcpy(data, pixels, (size_t) imageSize);
    }
    else
    {
        uint8_t* dataBytes = reinterpret_cast<uint8_t*>(data);

        for (int y = 0; y < texHeight; y++)
        {
            memcpy(&dataBytes[y * stagingImageLayout.rowPitch], &pixels[y * texWidth * 4], texWidth * 4);
        }
    }

    vkUnmapMemory(m_vkLogicalDevice, stagingImageMemory);

    stbi_image_free(pixels);

    CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_textureImage, m_textureImageMemory);

    TransitionImageLayout(stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyImage(stagingImage, m_textureImage, texWidth, texHeight);

    TransitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    if (stagingImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_vkLogicalDevice, stagingImageMemory, VK_NULL_HANDLE);
        stagingImageMemory = VK_NULL_HANDLE;
    }
    if (stagingImage != VK_NULL_HANDLE)
    {
        vkDestroyImage(m_vkLogicalDevice, stagingImage, nullptr);
        stagingImage = VK_NULL_HANDLE;
    }

    return true;
}

bool VulkanRenderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView)
{
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_vkLogicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        LOG_ERROR("failed to create texture image view!");
        return false;
    }
    return true;
}

bool VulkanRenderer::CreateTextureSampler()
{
    vp::system::Settings& settings = vp::system::Settings::Instance();

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = settings.GetAnisatropicFiltering();
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(m_vkLogicalDevice, &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create texture sampler!");
        return false;
    }
    return true;
}

bool VulkanRenderer::CreateTextureImageView()
{
    CreateImageView(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, m_textureImageView);

    return true;
}

bool VulkanRenderer::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_vkLogicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        LOG_ERROR("Failed to create image!");
        return false;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_vkLogicalDevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    if (!FindMemoryType(memRequirements.memoryTypeBits, properties, allocInfo.memoryTypeIndex))
        return false;
    if (vkAllocateMemory(m_vkLogicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        LOG_ERROR("failed to allocate image memory!");
        return false;
    }
    vkBindImageMemory(m_vkLogicalDevice, image, imageMemory, 0);
    return true;
}

bool VulkanRenderer::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

bool VulkanRenderer::CreateShaderModule(
    const std::vector<uint8_t>& code, VkShaderModule& shaderModule) const
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = (uint32_t*) code.data();
    if (vkCreateShaderModule(
            m_vkLogicalDevice, &createInfo, nullptr, &shaderModule) !=
        VK_SUCCESS)
    {
        LOG_ERROR("Failed to create shader module!");
        return false;
    }
    return true;
}

bool VulkanRenderer::IsDeviceSuitable(VkPhysicalDevice device)
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

bool VulkanRenderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
        device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(
        m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool VulkanRenderer::CreateBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory)
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

bool VulkanRenderer::CopyBuffer(
    VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
    return true;
}

bool VulkanRenderer::Frame()
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
        &presentInfo); // TODO : some error here if no vertex data provided?
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

std::vector<const char*> VulkanRenderer::GetRequiredExtensions()
{
    std::vector<const char*> m_extensions;
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions =
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

bool VulkanRenderer::SetupDebugCallback()
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
