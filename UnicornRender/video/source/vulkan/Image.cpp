/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Image.hpp>
#include <unicorn/video/vulkan/VulkanHelper.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Image::Image(vk::PhysicalDevice physicalDevice,
    vk::Device device,
    vk::Format format,
    vk::ImageUsageFlags usage,
    uint32_t width,
    uint32_t height
) : m_device(device)
    , m_image(nullptr)
    , m_deviceMemory(nullptr)
    , m_format(format)
    , m_width(width)
    , m_height(height)
    , m_initialized(false)
{
    m_usage = usage;

    vk::ImageCreateInfo imageInfo;
    imageInfo.setImageType(vk::ImageType::e2D);
    imageInfo.setFormat(m_format);
    imageInfo.extent.setWidth(m_width);
    imageInfo.extent.setHeight(m_height);
    imageInfo.extent.setDepth(1);
    imageInfo.setMipLevels(1);
    imageInfo.setArrayLayers(1);
    imageInfo.setSamples(vk::SampleCountFlagBits::e1);
    imageInfo.setUsage(m_usage);
    imageInfo.setSharingMode(vk::SharingMode::eExclusive);
    imageInfo.setQueueFamilyIndexCount(0);
    imageInfo.setPQueueFamilyIndices(nullptr);
    imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);

    vk::Result result = m_device.createImage(&imageInfo, nullptr, &m_image);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create Vulkan image!");
        return;
    }

    vk::MemoryRequirements req;
    m_device.getImageMemoryRequirements(m_image, &req);

    vk::PhysicalDeviceMemoryProperties memoryProperties;
    physicalDevice.getMemoryProperties(&memoryProperties);

    m_deviceMemory = new Memory(m_device, req.memoryTypeBits, memoryProperties, vk::MemoryPropertyFlagBits::eDeviceLocal, req.size);

    if(!m_deviceMemory->IsInitialized())
    {
        LOG_VULKAN->Error("Can't allocate memory for image!");
        return;
    }

    m_device.bindImageMemory(m_image, m_deviceMemory->GetMemory(), 0);

    vk::ImageAspectFlags aspect;
    if(m_usage & vk::ImageUsageFlagBits::eColorAttachment)
    {
        aspect |= vk::ImageAspectFlagBits::eColor;
    }
    if(m_usage & vk::ImageUsageFlagBits::eDepthStencilAttachment)
    {
        aspect |= vk::ImageAspectFlagBits::eDepth;
    }

    vk::ImageViewCreateInfo imageViewInfo;
    imageViewInfo.setFlags(vk::ImageViewCreateFlagBits());
    imageViewInfo.setImage(m_image);
    imageViewInfo.setFormat(m_format);
    imageViewInfo.components.setR(vk::ComponentSwizzle::eR);
    imageViewInfo.components.setG(vk::ComponentSwizzle::eG);
    imageViewInfo.components.setB(vk::ComponentSwizzle::eB);
    imageViewInfo.components.setA(vk::ComponentSwizzle::eA);
    imageViewInfo.subresourceRange.setAspectMask(aspect);
    imageViewInfo.subresourceRange.setBaseMipLevel(0);
    imageViewInfo.subresourceRange.setLevelCount(1);
    imageViewInfo.subresourceRange.setBaseArrayLayer(0);
    imageViewInfo.subresourceRange.setLayerCount(1);
    imageViewInfo.setViewType(vk::ImageViewType::e2D);

    result = m_device.createImageView(&imageViewInfo, nullptr, &m_imageView);
    if(result != vk::Result::eSuccess)
    {
        LOG_VULKAN->Error("Can't create Vulkan image view!");
        return;
    }
    m_initialized = true;
}

Image::~Image()
{
    if(m_imageView)
    {
        m_device.destroyImageView(m_imageView, nullptr);
        m_imageView = nullptr;
    }
    if(m_deviceMemory)
    {
        delete m_deviceMemory;
        m_deviceMemory = nullptr;
    }
    if(m_image)
    {
        m_device.destroyImage(m_image, nullptr);
        m_image = nullptr;
    }
}

bool Image::IsInitialized() const
{
    return m_initialized;
}

vk::Format Image::GetFormat() const
{
    return m_format;
}

uint32_t Image::GetWidth() const
{
    return m_width;
}

uint32_t Image::GetHeight() const
{
    return m_height;
}

const vk::Image& Image::GetVkImage() const
{
    return m_image;
}

const vk::ImageView& Image::GetVkImageView() const
{
    return m_imageView;
}

bool Image::TransitionLayout(const vk::Format& format,
    const vk::ImageLayout& oldLayout,
    const vk::ImageLayout& newLayout,
    const vk::CommandPool& cmdPool,
    const vk::Queue& queue) const
{
    vk::CommandBuffer commandBuffer = BeginSingleTimeCommands(m_device, cmdPool);

    vk::ImageMemoryBarrier barrier;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_image;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vk::PipelineStageFlags sourceStage;
    vk::PipelineStageFlags destinationStage;

    if(oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
    {
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

        sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
        destinationStage = vk::PipelineStageFlagBits::eTransfer;
    }
    else if(oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
    {
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        sourceStage = vk::PipelineStageFlagBits::eTransfer;
        destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
    }
    else
    {
        LOG_VULKAN->Error("Unsupported layout transition!");
        return false;
    }

    commandBuffer.pipelineBarrier(sourceStage,
                                  destinationStage,
                                  vk::DependencyFlagBits::eByRegion, 0,
                                  nullptr, 0,
                                  nullptr, 1,
                                  &barrier);

    EndSingleTimeCommands(commandBuffer, queue, m_device, cmdPool);
    return true;
}
}
}
}
