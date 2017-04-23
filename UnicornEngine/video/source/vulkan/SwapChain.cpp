/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/SwapChain.hpp>

namespace unicorn
{
    namespace video
    {
        namespace vulkan
        {
            Swapchain::Swapchain()
            {
            }

            Swapchain::~Swapchain()
            {
            }

            vk::Result Swapchain::Create(
                vk::PhysicalDevice physicalDevice,
                vk::Device device,
                vk::SurfaceKHR& surface,
                vk::ImageUsageFlags usage,
                uint32_t width,
                uint32_t height)
            {
                m_device = device;
                m_usage = usage;
                m_width = width;
                m_height = height;

                vk::SurfaceTransformFlagBitsKHR transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
                vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

                vk::Result result;

                uint32_t surfaceFormatsCount;
                result = physicalDevice.getSurfaceFormatsKHR(surface, &surfaceFormatsCount, nullptr);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }
                std::vector<vk::SurfaceFormatKHR> surfaceFormats(surfaceFormatsCount);
                result = physicalDevice.getSurfaceFormatsKHR(surface, &surfaceFormatsCount, surfaceFormats.data());
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                m_format = surfaceFormats[0].format;

                uint32_t surfacePresentModesCount;
                result = physicalDevice.getSurfacePresentModesKHR(surface, &surfacePresentModesCount, nullptr);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }
                std::vector<vk::PresentModeKHR> surfacePresentModes(surfacePresentModesCount);
                result = physicalDevice.getSurfacePresentModesKHR(surface, &surfacePresentModesCount, surfacePresentModes.data());
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                //get SurfaceCapablilies
                vk::SurfaceCapabilitiesKHR surfaceCapabilities;
                result = physicalDevice.getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                if (!(surfaceCapabilities.supportedUsageFlags & usage))
                {
                    return vk::Result::eErrorInitializationFailed;
                }
                if (!(surfaceCapabilities.supportedTransforms & transform))
                {
                    return vk::Result::eErrorInitializationFailed;
                }
                if (!(surfaceCapabilities.supportedCompositeAlpha & compositeAlpha))
                {
                    return vk::Result::eErrorInitializationFailed;
                }

                vk::SwapchainCreateInfoKHR swapchainInfo;
                swapchainInfo.setSurface(surface);
                swapchainInfo.setMinImageCount(surfaceCapabilities.minImageCount);
                swapchainInfo.setImageFormat(surfaceFormats[0].format);
                swapchainInfo.setImageColorSpace(surfaceFormats[0].colorSpace);
                swapchainInfo.setImageExtent(vk::Extent2D(width, height));
                swapchainInfo.setImageArrayLayers(1);
                swapchainInfo.setImageUsage(m_usage);
                swapchainInfo.setImageSharingMode(vk::SharingMode::eExclusive);
                swapchainInfo.setQueueFamilyIndexCount(0);
                swapchainInfo.setPQueueFamilyIndices(nullptr);
                swapchainInfo.setPreTransform(transform);
                swapchainInfo.setCompositeAlpha(compositeAlpha);
                swapchainInfo.setPresentMode(surfacePresentModes[0]);
                swapchainInfo.setClipped(true);
                swapchainInfo.setOldSwapchain(vk::SwapchainKHR());

                result = m_device.createSwapchainKHR(&swapchainInfo, nullptr, &m_swapchain);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                uint32_t swapchainCount;
                result = m_device.getSwapchainImagesKHR(m_swapchain, &swapchainCount, nullptr);
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }
                m_images.resize(swapchainCount);
                result = m_device.getSwapchainImagesKHR(m_swapchain, &swapchainCount, m_images.data());
                if (result != vk::Result::eSuccess)
                {
                    return result;
                }

                m_imageViews.resize(swapchainCount);
                vk::ImageAspectFlags aspect;
                if (m_usage & vk::ImageUsageFlagBits::eColorAttachment)
                {
                    aspect |= vk::ImageAspectFlagBits::eColor;
                }
                if (m_usage & vk::ImageUsageFlagBits::eDepthStencilAttachment)
                {
                    aspect |= vk::ImageAspectFlagBits::eDepth;
                }
                for (uint32_t i = 0; i<swapchainCount; ++i)
                {
                    vk::ImageViewCreateInfo imageViewInfo;
                    imageViewInfo.setImage(m_images[i]);
                    imageViewInfo.setViewType(vk::ImageViewType::e2D);
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

                    result = m_device.createImageView(&imageViewInfo, nullptr, &m_imageViews[i]);
                    if (result != vk::Result::eSuccess)
                    {
                        return result;
                    }
                }
                return result;
            }

            vk::ImageUsageFlags Swapchain::GetUsage()
            {
                return m_usage;
            }
            vk::Format Swapchain::GetFormat()
            {
                return m_format;
            }
            uint32_t Swapchain::GetWidth()
            {
                return m_width;
            }
            uint32_t Swapchain::GetHeight()
            {
                return m_height;
            }
            size_t Swapchain::Count()
            {
                return m_images.size();
            }
            vk::SwapchainKHR& Swapchain::GetVkSwapchainKHR()
            {
                return m_swapchain;
            }
            vk::Image& Swapchain::GetVkImage(size_t index)
            {
                return m_images[index];
            }
            vk::ImageView& Swapchain::GetVkImageView(size_t index)
            {
                return m_imageViews[index];
            }
        }
    }
}