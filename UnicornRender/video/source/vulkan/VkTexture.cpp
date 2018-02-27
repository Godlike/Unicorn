/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VkTexture.hpp>
#include <unicorn/video/Texture.hpp>

#include <unicorn/utility/InternalLoggers.hpp>

namespace unicorn
{
namespace video
{
namespace vulkan
{
VkTexture::VkTexture(vk::Device device)
    : m_device(device)
    , m_vkImage(nullptr)
    , m_isInitialized(false)
{
}

const vk::DescriptorImageInfo& VkTexture::GetDescriptorImageInfo() const
{
    return m_imageInfo;
}

bool VkTexture::Create(const vk::PhysicalDevice& physicalDevice, const vk::Device& device,
                       const vk::CommandPool& commandPool, const vk::Queue& queue, Texture const& texture)
{
    if(!m_isInitialized)
    {
        Buffer imageStagingBuffer;
        bool result = imageStagingBuffer.Create(
            physicalDevice, device,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
            texture.Size());
        if(!result)
        {
            LOG_VULKAN->Error("Can't allocate staging buffer for texture - {}", texture.Path().c_str());
            return false;
        }

        imageStagingBuffer.Map();
        imageStagingBuffer.Write(texture.Data());
        imageStagingBuffer.Unmap();

        m_vkImage = new Image(
            physicalDevice,
            device,
            vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment,
            texture.Width(),
            texture.Height());
        if(!m_vkImage->IsInitialized())
        {
            LOG_VULKAN->Error("Can't allocate vulkan based image for texture - {}", texture.Path().c_str());
            delete m_vkImage;
            m_vkImage = nullptr;
            return false;
        }

        m_vkImage->TransitionLayout(vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined,
                                    vk::ImageLayout::eTransferDstOptimal, commandPool, queue);
        imageStagingBuffer.CopyToImage(*m_vkImage, commandPool, queue);
        m_vkImage->TransitionLayout(vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal,
                                    vk::ImageLayout::eShaderReadOnlyOptimal, commandPool, queue);

        imageStagingBuffer.Destroy();

        vk::SamplerCreateInfo samplerInfo;
        samplerInfo.setMagFilter(vk::Filter::eLinear);
        samplerInfo.setMinFilter(vk::Filter::eLinear);
        samplerInfo.setAddressModeU(vk::SamplerAddressMode::eRepeat);
        samplerInfo.setAddressModeV(vk::SamplerAddressMode::eRepeat);
        samplerInfo.setAddressModeW(vk::SamplerAddressMode::eRepeat);
        samplerInfo.setAnisotropyEnable(VK_TRUE);
        samplerInfo.setMaxAnisotropy(16);
        samplerInfo.setBorderColor(vk::BorderColor::eIntOpaqueBlack);
        samplerInfo.setUnnormalizedCoordinates(VK_FALSE);
        samplerInfo.setCompareEnable(VK_FALSE);
        samplerInfo.setCompareOp(vk::CompareOp::eAlways);
        samplerInfo.setMipmapMode(vk::SamplerMipmapMode::eLinear);
        samplerInfo.setMipLodBias(0.0f);
        samplerInfo.setMinLod(0.0f);
        samplerInfo.setMaxLod(0.0f);

        result = device.createSampler(&samplerInfo, nullptr, &m_sampler) == vk::Result::eSuccess;

        if(!result)
        {
            LOG_VULKAN->Error("Can't create sampler for texture - {}", texture.Path().c_str());
            delete m_vkImage;
            m_vkImage = nullptr;
            return false;
        }

        m_imageInfo.sampler = m_sampler;
        m_imageInfo.imageView = m_vkImage->GetVkImageView();
        m_isInitialized = true;
    }
    return true;
}

void VkTexture::Delete()
{
    if(m_isInitialized)
    {
        if(m_device && m_sampler)
        {
            m_device.destroySampler(m_sampler);
        }

        delete m_vkImage;
        m_vkImage = nullptr;
        m_isInitialized = false;
    }
}

bool VkTexture::IsInitialized() const
{
    return m_isInitialized;
}
}
}
}
