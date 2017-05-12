/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/Image.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
Image::Image() : m_format(), m_width(0), m_height(0)
{
}

Image::~Image()
{
}

vk::Result Image::Create(
    vk::Device device,
    vk::Format format,
    vk::ImageUsageFlags usage,
    uint32_t width,
    uint32_t height)
{
    m_device = device;
    m_format = format;
    m_usage = usage;
    m_width = width;
    m_height = height;

    vk::ImageCreateInfo imageInfo;
    imageInfo.setImageType(vk::ImageType::e2D);
    imageInfo.setFormat(m_format);
    imageInfo.extent.setWidth(m_width);
    imageInfo.extent.setHeight(m_height);
    imageInfo.extent.setDepth(1);
    imageInfo.setMipLevels(1);
    imageInfo.setArrayLayers(1);
    imageInfo.setSamples(vk::SampleCountFlagBits::e1);
    imageInfo.setTiling(vk::ImageTiling::eLinear);
    imageInfo.setUsage(m_usage);
    imageInfo.setSharingMode(vk::SharingMode::eExclusive);
    imageInfo.setQueueFamilyIndexCount(0);
    imageInfo.setPQueueFamilyIndices(nullptr);
    imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);

    vk::Result result = m_device.createImage(&imageInfo, nullptr, &m_image);

    vk::MemoryRequirements req;
    m_device.getImageMemoryRequirements(m_image, &req);
    vk::MemoryAllocateInfo allocInfo;
    allocInfo.setAllocationSize(req.size);
    allocInfo.setMemoryTypeIndex(1);

    result = m_device.allocateMemory(&allocInfo, nullptr, &m_deviceMemory);

    m_device.bindImageMemory(m_image, m_deviceMemory, 0);

    vk::ImageAspectFlags aspect;
    if ( m_usage & vk::ImageUsageFlagBits::eColorAttachment )
    {
        aspect |= vk::ImageAspectFlagBits::eColor;
    }
    if ( m_usage & vk::ImageUsageFlagBits::eDepthStencilAttachment )
    {
        aspect |= vk::ImageAspectFlagBits::eDepth;
    }

    vk::ImageViewCreateInfo imageViewInfo;
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
    return result;
}

void Image::Destroy() const
{
    if ( m_imageView )
    {
        m_device.destroyImageView(m_imageView);
    }
    if ( m_deviceMemory )
    {
        m_device.freeMemory(m_deviceMemory);
    }
    if ( m_image )
    {
        m_device.destroyImage(m_image);
    }
}

vk::Format Image::GetFormat() const
{
    return m_format;
}

vk::Image& Image::GetVkImage()
{
    return m_image;
}
vk::ImageView& Image::GetVkImageView()
{
    return m_imageView;
}
int32_t Image::GetWidth() const
{
    return m_width;
}
int32_t Image::GetHeight() const
{
    return m_height;
}
}
}
}