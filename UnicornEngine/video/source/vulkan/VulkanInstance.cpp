/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include <unicorn/video/vulkan/VulkanInstance.hpp>
#include <unicorn/Settings.hpp>
#include <unicorn/utility/Logger.hpp>

namespace unicorn
{
namespace video
{
bool VulkanInstance::CreateInstance()
{
    if (s_enableValidationLayers && !CheckValidationLayerSupport())
    {
        LOG_ERROR("Vulkan validation layers requested, but not available!");
        return false;
    }

    const Settings& settings = Settings::Instance();

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

VulkanInstance::VulkanInstance()
{
    CreateInstance();
}

VulkanInstance::~VulkanInstance()
{
    DestroyInstance();
}

bool VulkanInstance::CheckValidationLayerSupport() const
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

void VulkanInstance::DestroyInstance()
{
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions()
{
    std::vector<const char*> extensions(m_systemManager.GetRequiredVulkanExtensions());

    if (s_enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}
}
}
